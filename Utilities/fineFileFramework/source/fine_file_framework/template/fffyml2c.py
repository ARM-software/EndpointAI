#!/usr/bin/python
# -*- coding: utf-8 -*-

# *************************************************************************************************
#  fineFileFramework project
#  @file        yml2c.py
#  @brief       yml to C converter
#
# *************************************************************************************************
#
# * Copyright (C) 2010-2023 ARM Limited or its affiliates. All rights reserved.
# *
# * SPDX-License-Identifier: Apache-2.0
# *
# * Licensed under the Apache License, Version 2.0 (the License); you may
# * not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an AS IS BASIS, WITHOUT
# * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# */

import sys
import os
import time
import re
import operator
from argparse import ArgumentParser
from collections import OrderedDict
from colorama import init as cinit, Fore, Back, Style

from enum import Enum
from pathlib import Path
from yaml import safe_load





class FffEltType(Enum):
    """ fff items """
    FILE = 1
    FOLDER = 2
    DISK = 3


def str_2_size_in_byte(size, unit):
    """
        convert yaml size string in bytes
        accept int, float size
        unit in K, M or no unit

    """
    assert isinstance(size, str) and isinstance(unit, str)
    assert unit in ['K', 'M', None]

    scale = 1024 * 1024 if unit == 'M' else 1024
    return int(scale * float(size))


def alias_to_node(alias):
    """ convert alias into lower case and replace slashes with underscores """
    return alias.lower().replace("/", "_").replace('\\', '_').replace(".", "_")


def incbin_prefix(name):
    """ convert name into upper case and replace slashes with underscores for incbin reference"""
    return name.upper().replace("/", "_").replace('\\', '_').replace(".", "_")


def disk_to_struct(disk_path):
    """ convert disk path structure name"""
    return disk_path.lower().replace(":", "").replace("/", "_").replace(
        '\\', '_').replace(".", "_")


class FffHostDirAttr:
    class FffHostDirFilter:
        ''' File Filtering for Host mapped folders '''

        def __init__(self, val):
            self._size = None
            self._name = None

            key_to_attr = {'Size-Filter': '_size', 'Name-Filter': '_name'}

            for i in val:
                if isinstance(i, dict):
                    k, v = list(i.items())[0]
                    setattr(self, key_to_attr.get(k), v.split())

            # filter comparison operator + combination
            self.ops = {
                "<": operator.lt,
                "<=": operator.le,
                "==": operator.eq,
                ">": operator.gt,
                ">=": operator.ge,
                "AND": operator.and_,
                "OR": operator.or_
            }

            assert self._size is None or self._size[0] in [
                'AND', 'OR'
            ], f'invalid size filter {self._size}'
            assert self._name is None or self._name[0] in [
                'AND', 'OR'
            ], f'invalid name filter {self._name}'

            # parse size filter
            self._filter_size = 0
            self._size_cmp = None
            if self._size:
                cmpre = r'"?(?P<cmp>([<>]=?|==))(?P<size>((\d+(\.\d+)?)|(\.\d+)))(?P<unit>[KM]?)"?'
                cmpre = re.compile(cmpre)
                size_compare = cmpre.match(self._size[1])
                assert size_compare, f'invalid size filter {self._size}'

                self._filter_size = str_2_size_in_byte(
                    size_compare.group('size'), size_compare.group('unit'))
                self._size_cmp = self.ops[size_compare.group('cmp')]

            # extract name filer
            if self._name:
                self._name_compare = re.compile(self._name[1])

        def __size_cmp__(self, size):
            """ compare filesize with filter size       """

            return self._size_cmp(size, self._filter_size)

        def condition_match(self, filename):
            """ filtering compare operator, returns True if file meet filters criteria """
            if not os.path.exists(filename):
                return False

            filesize = os.path.getsize(filename)
            filename = os.path.basename(filename)

            match_size = True if self.__size_cmp__(
                filesize) and self._size else False
            match_name = True if self._name_compare.match(
                filename) and self._name else False

            match = True
            # size filter check
            if self._size:
                match = self.ops[self._size[0]](match, match_size)

            # name filter check
            if self._name:
                match = self.ops[self._name[0]](match, match_name)

            return match

        def __repr__(self):
            return f"Filter: size={self._size} name={self._name}"

    def __init__(self, val):
        """
            Host directory attribute constructor, expect dictionnary with
            Source : Host folder path
            Access : gloabl access for all files
            Recursive : boolean for enabling full directory scan
            Path : Top level aliases list
            Filters : filter dictionnary
            Exclude-Files : list of of file to exclude (full path)

        """
        assert isinstance(
            val, dict), "FffHostDirAttr expect an input dictionnary !!"

        self._root = val.get('Source')
        self._recursive = val.get('Recursive') == "YES"
        self._alias = []

        access = val.get('Access')
        self._access = "FFF_" + str(access).replace(
            " ", "_"
        ) if access is not None else FffYmltoCFsConv.Defaults.FFF_HOST_FILE_ACCESS

        path_list = val.get('Path')
        # aliases optional
        if len(path_list) > 0:
            try:
                assert isinstance(
                    path_list[0],
                    dict), "Folder is not a dict, YML to be fixed !!"
            except AssertionError as msg:
                print(msg)
                print(f'offending entry : {val}')
                sys.exit(1)

            self._alias = [i.get('Alias') for i in path_list]

        self._filter = val.get('Filters')
        if self._filter:
            self._filter = FffHostDirAttr.FffHostDirFilter(self._filter)

        self._exclusion_list = []
        exclusion_list = val.get('Exclude-Files')
        # exclusion_list optional
        if len(exclusion_list) > 0:
            try:
                assert isinstance(
                    exclusion_list[0],
                    dict), "Folder is not a dict, YML to be fixed !!"
            except AssertionError as msg:
                print(msg)
                print(f'offending entry : {val}')
                sys.exit(1)

            # expect full path
            self._exclusion_list = [i.get('File') for i in exclusion_list]

    @property
    def root(self):
        return self._root

    @property
    def recursive(self):
        return self._recursive

    @property
    def alias(self):
        return self._alias

    @property
    def access(self):
        return self._access

    @property
    def exclusion_list(self):
        return self._exclusion_list

    @property
    def filter(self):
        return self._filter

    def __repr__(self):
        return f"FFFHostDir: root={self._root} recurs={self._recursive} access={self._access} alias={self._alias} exc={self._exclusion_list}"


class FffHostFileTree:
    """
        Host directory tree
        start with top level path.
        if recursive flag is Flase, scan depth is limited to 0
    """

    def __init__(self, path, recursive, isdir='True'):

        self._name = path
        self._isdir = isdir
        self._children = []
        if os.path.isdir(path):
            for child_name in os.listdir(path):
                child_path = os.path.join(path, child_name)
                child_is_dir = os.path.isdir(child_path)
                if recursive or not child_is_dir:
                    self._children.append(
                        FffHostFileTree(child_path, recursive, child_is_dir))

    @property
    def isdir(self):
        return self._isdir

    @property
    def name(self):
        return self._name

    @property
    def children(self):
        return self._children

    def __repr__(self):
        if self._isdir:
            return f"Directory {self._name}"
        return f"File {self._name}"


class FffFolderAttrib:
    """
        Folder attributes with
        Node : converted alias into FFF node name (fff_folder)
        Alias list
    """

    def __init__(self, alias, node):

        self._node = node
        self._alias = alias


    @property
    def node(self):
        return self._node

    @property
    def alias(self):
        return self._alias



class FffFileAttrib:
    """
        File attributes with
        Node : FFF file node name (arm_mem_file_node_t)
        Access : File access
        Size, Array, Section for memory files
        inc_bin_ref for host mapped files

    """

    def __init__(self,
                 path,
                 node,
                 access,
                 incbin,
                 size_arr_section=(None, None, None)):
        self.path = path
        self._node = node
        if access is not None and 'FFF' in access:
            self._access = access
        else:
            self._access = "FFF_" + str(access).replace(
                " ", "_"
            ) if access is not None else FffYmltoCFsConv.Defaults.FFF_ACCESS
        assert self._access in ['FFF_READ_ONLY', 'FFF_READ_WRITE'
                                ], f'invalid access {self._access}'
        size, array, section = size_arr_section
        self.section = section if section != "" else FffYmltoCFsConv.Defaults.FFF_SECTION
        self.array = array
        self.size = size
        self.inc_bin_ref = incbin if incbin is not None else "NONE"

    @property
    def node(self):
        return self._node

    @property
    def access(self):
        return self._access

    def __repr__(self):
        return f"file | {self.path} | {self.node} |   {self._access} |  {self.inc_bin_ref} | {self.section} | {self.size}"





class FffYmltoCFsConv:
    """
        YML to C (Fine) File System Conversion master class
    """

    class Defaults:
        """ FFF defaults behaviour  """
        FFF_DRIVE_NAME = "Local_Disk"
        FFF_WORKING_PATH = "."
        FFF_ACCESS = "FFF_READ_WRITE"
        FFF_HOST_FOLDER_ACCESS = "FFF_READ_WRITE"
        FFF_HOST_FILE_ACCESS = "FFF_READ_ONLY"
        FFF_DUMMY_INS_EMPTY_FOLDER = True
        FFF_SECTION = ".data"
        FFF_FILE_SECTION = ".text"
        FFF_SECTION_SIZE = 1024 * 1024

        FFF_C_HDR = """
/* Generated on {0} from {1} */



#ifdef __FFF_USER_DEFINE_FILE_FRAMEWORK__
#undef __FFF_USER_DEFINE_FILE_FRAMEWORK__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/


"""

        ### TOCHECK default RODATA_AREA ???

        FFF_INCBIN_PART_HDR = """

    /* beg of incbin generated part */

#define INCBIN_PREFIX
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#define INCBIN_OUTPUT_SECTION   "{0}"
#include "incbin.h"

        """

        FFF_CMDLINE_PART_HDR = """

__attribute__((used))
char *_sys_command_string(char *cmd, int len)
{
    (void)cmd;
    (void)len;
    /* write a command line here, which will be passed to main */
    """

    class FffStats:
        """ misc statistics """

        def __init__(self):
            self.total_source_files = 0
            self.total_static_mem_files = 0
            self.total_heap_mem_files = 0
            self.total_source_files_mem = 0
            self.total_static_mem_files_mem = 0

        def __str__(self):
            return f'''
        /*
            ~~~~~~ FFF Statistics ~~~~~~~

            Total source files          = {self.total_source_files}
            Total source files memory   = {self.total_source_files_mem} bytes
            Total static memory files   = {self.total_static_mem_files}
            Total static memory         = {self.total_static_mem_files_mem} bytes
            Total files using heap      = {self.total_heap_mem_files}

        */'''

    def __init__(self, yaml_tree, output):

        self._yaml_tree = yaml_tree

        self._drive_name = FffYmltoCFsConv.Defaults.FFF_DRIVE_NAME
        self._working_path = FffYmltoCFsConv.Defaults.FFF_WORKING_PATH
        self._file_section = FffYmltoCFsConv.Defaults.FFF_FILE_SECTION
        self._file_section_size = FffYmltoCFsConv.Defaults.FFF_SECTION_SIZE
        self._arglist = []
        self._drive_list = []

        self._inc_bin_list = []
        self._mem_size_section_list = []
        self.output = output
        self._memory_limit_reached = False

        self._fff_node_hierarchy = OrderedDict()

        self._stats = FffYmltoCFsConv.FffStats()
        # top level items parsing
        self._fff_parse_toplvl()




    @property
    def drive_name(self):
        return self._drive_name

    @drive_name.setter
    def drive_name(self, value):
        if value != '':
            self._drive_name = value

    @property
    def memory_limit_reached(self):
        return self._memory_limit_reached

    @memory_limit_reached.setter
    def memory_limit_reached(self, value):
        self._memory_limit_reached = value

    @property
    def working_path(self):
        return self._working_path

    @working_path.setter
    def working_path(self, value):
        if value != '':
            self._working_path = value

    @property
    def fff_node_hierarchy(self):
        return self._fff_node_hierarchy


    def fff_node_hierarchy_insert(self, key, value):
        if self._fff_node_hierarchy.get(key) is None:
            self._fff_node_hierarchy[key] = []

        self._fff_node_hierarchy[key].append(value)


    def fff_write(self, string):
        print(string, file=self.output)


    def _fff_parse_toplvl(self):
        # drive name, file section/size limit extraction
        root = self._yaml_tree.get('Root')
        assert isinstance(root, dict), 'invalid Yaml toplevel'
        for k, v in root.items():

            # handle section / max size for sources
            if k == 'Memory':
                filesection = v.get('Section')
                if filesection:
                    self._file_section = filesection

                file_section_size = v.get('Size')
                if file_section_size:
                    szre = re.compile(
                        r'"?(?P<size>((\d+(\.\d+)?)|(\.\d+)))(?P<unit>[KM]?)"?'
                    )
                    size = szre.match(file_section_size)
                    assert size, f'invalid section size  {file_section_size}'

                    self._file_section_size = str_2_size_in_byte(
                        size.group('size'), size.group('unit'))

            elif k == 'Name':
                self.drive_name = v

            elif k == 'WorkingPath':
                self.working_path = v

        # CMD Line arg list
        cmdline = self._yaml_tree.get('Command-line')
        for i in cmdline:
            if isinstance(i, dict):
                arg = i.get('Arg')
                if arg:
                    self._arglist.append(arg)

    def _fff_scan_host_folder(self, path, host_dir_attr, parent, indent=0):
        """ scan host folder, apply filtering and attach folder / file attribute to main class"""
        if path.isdir:

            # for host, folder aliases & nodes are created from full path folder names
            alias = path.name
            node = alias_to_node(alias)
            folder_struct_path = f'{parent}.{node}'


        for child in path.children:

            if child.isdir:
                alias = child.name
                node = alias_to_node(child.name)

                # get directory tail
                cur_fold = Path(alias).parts[-1]

                folder_alias = [cur_fold]
                # if root directiory has aliases, replace
                if child.name == host_dir_attr.root:
                    folder_alias = host_dir_attr.alias

                folder = FffFolderAttrib(folder_alias, node)

                self.fff_node_hierarchy_insert(folder_struct_path,
                    (FffEltType.FOLDER,
                     (node, folder, f'{folder_struct_path}.{node}',
                      folder_struct_path)))

                # scan one level down
                self._fff_scan_host_folder(child, host_dir_attr,
                                           folder_struct_path, indent + 1)

            else:

                folder_node = alias_to_node(path.name)
                # nested struct
                folder_struct_path = f'{parent}.{folder_node}'

                cur_file = child.name

                # filtering checks
                if cur_file not in host_dir_attr.exclusion_list and     \
                    host_dir_attr.filter.condition_match(cur_file) and  \
                    not self.memory_limit_reached :

                    file_size = os.path.getsize(cur_file)
                    if self._stats.total_source_files_mem + file_size > self._file_section_size:
                        max_sz = float(self._file_section_size / 1024)
                        print(
                            f'Total source size limit reached {max_sz:.1f}K, discard subsequent source files'
                        )
                        self.memory_limit_reached = True
                        continue

                    if file_size == 0:
                        print(Fore.YELLOW +
                            f'Source file {cur_file} size = 0 byte skipping...' +
                            Style.RESET_ALL)

                        continue

                    self._stats.total_source_files += 1
                    self._stats.total_source_files_mem += file_size

                    path_to_file = cur_file
                    file_node = alias_to_node(path_to_file)

                    assert path_to_file is not None, "path error"
                    self._inc_bin_list.append(path_to_file)

                    file = FffFileAttrib(path_to_file, file_node,
                                         host_dir_attr.access, path_to_file)

                    self.fff_node_hierarchy_insert(folder_struct_path,
                        (FffEltType.FILE, (file_node, file, folder_struct_path)))



    def fff_yml_2_tree(self, data, path='', level=0):
        """  yml scanner entry"""
        if isinstance(data, dict):

            for key, val in data.items():
                struct_postfix = ''

                if key == 'Root':
                    struct_postfix = f'&{self.drive_name}'


                elif key == 'Disk':
                    disk_path = val['Path']
                    disk_drive = disk_to_struct(disk_path)
                    struct_postfix = f'.disk_{disk_drive}'
                    self._drive_list.append((disk_drive, disk_path,
                                             f'{path}{struct_postfix}'))
                    self.fff_node_hierarchy_insert(path,
                        (FffEltType.DISK,
                         (f'disk_{disk_drive}', f'{path}{struct_postfix}',
                          disk_path, path)))

                elif key == 'Folder':

                    # host folder
                    if val.get('Source') is not None:
                        dir_top = val.get('Source')

                        if not os.path.isdir(dir_top):
                            print(f'Host directory {dir_top} not found')
                            return

                        # set host dir attributes + filtering
                        host_dir_attr = FffHostDirAttr(val)

                        # build host tree
                        local_dir = FffHostFileTree(dir_top,
                                                    host_dir_attr.recursive)

                        node = alias_to_node(dir_top)

                        self.fff_node_hierarchy_insert(path,
                            (FffEltType.FOLDER, (node, host_dir_attr, f'{path}.{node}',
                                              path)))

                        # scan files, retrieve inbin ref + filtering
                        self._fff_scan_host_folder(local_dir, host_dir_attr,
                                                   path)

                        return

                    cur_folder = val
                    path_list = cur_folder.get('Path')
                    assert len(
                        path_list
                    ) > 0, "!! Folder path without 'Path' entry, YML to be fixed !!"

                    try:
                        assert isinstance(
                            path_list[0],
                            dict), "Folder is not a dict, YML to be fixed !!"
                    except AssertionError as msg:
                        print(msg)
                        print(f'offending entry : {val}')
                        sys.exit(1)

                    # use 1st alias to derive node name
                    alias_list = [i.get('Alias') for i in path_list]
                    alias = alias_list[0]
                    assert alias != "", "!! Folder path without 'Alias' entry, YML to be fixed !!"

                    node = alias_to_node(alias)
                    struct_postfix = f'.{node}'

                    folder_access = cur_folder.get('Access')
                    if folder_access is not None:
                        print(f'Folder access set {folder_access} for {alias}: ignored')

                    folder = FffFolderAttrib(alias_list, node)

                    self.fff_node_hierarchy_insert(path,
                            (FffEltType.FOLDER,
                             (node, folder, f'{path}{struct_postfix}', path)))

                elif key == 'File':

                    path_to_file = val.get('Path')
                    node_ref = alias_to_node(path_to_file)

                    inc_bin_ref = None
                    source_ref = val.get('Source')
                    if source_ref:

                        if os.path.exists(source_ref):
                            file_size = os.path.getsize(source_ref)
                            if self._stats.total_source_files_mem + file_size > self._file_section_size:
                                print(
                                    'Total source size limit reached, discard subsequent source files'
                                )
                                self.memory_limit_reached = True
                                continue
                            if os.path.getsize(source_ref) == 0:
                                print(Fore.YELLOW +
                                    f'Source file {source_ref} size = 0 skipping...' +
                                    Style.RESET_ALL)
                                continue
                        else:
                            print(Fore.RED +
                                f'Source file {source_ref} does not exist, skipping...' +
                                Style.RESET_ALL)
                            continue

                        self._stats.total_source_files += 1
                        self._stats.total_source_files_mem += file_size

                        self._inc_bin_list.append(source_ref)
                        inc_bin_ref = source_ref

                    mem_ref = val.get('Memory')
                    size_name, array_name, mem_section = (None, None, None)
                    if mem_ref:
                        mem_size = mem_ref.get('Size')
                        mem_section = mem_ref.get('Section')
                        assert mem_size is not None, f'Invalid Memory entry, no size {mem_ref}'
                        assert mem_section is not None, f'Invalid Memory entry, no section {mem_ref}'

                        # build defines, array and size globals for memory files
                        size_cst = node_ref.upper() + '_SZ'
                        size_name = node_ref + '_size'
                        array_name = node_ref + '_arr'

                        self._stats.total_static_mem_files_mem += mem_size

                        self._mem_size_section_list.append(
                            (size_cst, size_name, array_name, mem_size,
                             mem_section))

                    if not inc_bin_ref:
                        if mem_ref:
                            self._stats.total_static_mem_files += 1
                        else:
                            self._stats.total_heap_mem_files += 1


                    file = FffFileAttrib(path_to_file, node_ref,
                                         val.get('Access'), inc_bin_ref,
                                         (size_name, array_name, mem_section))


                    self.fff_node_hierarchy_insert(path,
                        (FffEltType.FILE, (node_ref, file, path)))

                # continue next level scan
                self.fff_yml_2_tree(val, f'{path}{struct_postfix}', level + 1)



        elif isinstance(data, list):

            for item in data:
                self.fff_yml_2_tree(item, f'{path}', level + 1)

    def fff_gen_root_pwd(self):
        """ export ROOT / WORKING_PATH """
        self.fff_write(
            f'#define __FFF_CFG_ROOT__            {self.drive_name}')
        self.fff_write(
            f'#define __FFF_CFG_WORKING_PATH__    "{self.working_path}"\n\n')


    def fff_gen_incbin(self):
        """Generate incbin aprt using source reference + tag"""

        src_list = self._inc_bin_list

        if not src_list:
            return

        self.fff_write(
            FffYmltoCFsConv.Defaults.FFF_INCBIN_PART_HDR.format(
                self._file_section))

        inc_bin_list = zip([incbin_prefix(v) for v in src_list], src_list)

        for tag, src in inc_bin_list:
            self.fff_write(f'INCBIN({tag}, "RTE/File_System/{src}");')

        self.fff_write("""

    /* end of incbin part */

        """)


    def fff_gen_array_section(self):
        """Generate incbin aprt using source reference + tag"""

        self.fff_write('''

    /* static arrays with section definition */

    ''')

        for (size_cst, size_name, array_name, mem_size,
             mem_section) in self._mem_size_section_list:

            self.fff_write(f'#define {size_cst}\t({mem_size})')
            self.fff_write(f'const uint32_t {size_name}= {size_cst};')
            self.fff_write(
                f'__attribute__ ((section("{mem_section}")))  uint8_t {array_name}[{size_cst}];\n\n'
            )

        self.fff_write("""

    /* end of static arrays with section definition part */

        """)

    def fff_gen_cmdline(self):
        """Generate incbin aprt using source reference + tag"""

        self.fff_write(FffYmltoCFsConv.Defaults.FFF_CMDLINE_PART_HDR)

        arglist = " ".join(self._arglist)
        self.fff_write(f'\treturn "./app {arglist}";\n}}\n\n')


    def fff_prolog(self, yml):

        self.fff_write(
            FffYmltoCFsConv.Defaults.FFF_C_HDR.format(
                time.asctime(time.localtime(time.time())), yml))


    def fff_epilog(self):

        self.fff_write(self._stats)

        self.fff_write("\n#endif /* __FFF_USER_DEFINE_FILE_FRAMEWORK__ */")




    def fff_gen_def(self):
        """ generate node list tree definition (def_fff) """
        def _fff_gen_node_list(entry, cur, lev=1):

            indent = " " * lev * 4

            for i, x in cur:
                if i == FffEltType.FOLDER:
                    (node, _, key, _) = x
                    if entry.get(key):
                        self.fff_write(f'{indent}use_fff_folder_begin({node})')
                        _fff_gen_node_list(entry, entry[key], lev + 1)
                        self.fff_write(
                            f'{indent}use_fff_folder_end({node}); /* (lvl {lev}) */')
                    else:
                        # empty folder : insert dummy node
                        # TODO : add empty folder removal option
                        assert FffYmltoCFsConv.Defaults.FFF_DUMMY_INS_EMPTY_FOLDER is True, \
                            "only dummy node insertion supported in this version"
                        if FffYmltoCFsConv.Defaults.FFF_DUMMY_INS_EMPTY_FOLDER:
                            self.fff_write(f'{indent}use_fff_folder_begin({node}) /* empty */')
                            self.fff_write(f'{indent}\tarm_mem_file_node_t\tdummy_node;')
                            self.fff_write(f'{indent}use_fff_folder_end({node});')


                elif i == FffEltType.FILE:
                    (node, _, _) = x
                    self.fff_write(f'{indent}arm_mem_file_node_t\t{node};')
                    continue


                elif i == FffEltType.DISK:
                    (node, key, _, _) = x
                    if entry.get(key):
                        self.fff_write(f'{indent}use_fff_disk_begin({node})')
                        _fff_gen_node_list(entry, entry[key], lev + 1)
                        self.fff_write(f'{indent}use_fff_disk_end({node});')


        self.fff_write(f'def_fff_begin({self.drive_name})')

        root = self.fff_node_hierarchy
        cur = next(iter(((root).items())))
        _fff_gen_node_list(root, cur[1], 1)

        self.fff_write(f'def_fff_end({self.drive_name});\n\n')


    def fff_gen_imp(self):
        """ generate node list implementation details (def_fff) """
        def _get_summmary(cur):

            ordered_list = []
            folder_cnt = 0
            file_cnt = 0
            disk_cnt = 0

            for i, x in cur:

                if i == FffEltType.FOLDER:
                    (node, _, _, _) = x
                    folder_cnt += 1
                    ordered_list.append(node)

                elif i == FffEltType.DISK:
                    (node, _, _, _) = x
                    disk_cnt += 1
                    ordered_list.append(node)

                elif i == FffEltType.FILE:
                    (node, _, _) = x
                    file_cnt += 1
                    ordered_list.append(node)

            return (folder_cnt, file_cnt, disk_cnt, ordered_list)

        def _get_next_node(node_list, cur_node):
            """  return next folder node given a current folder and parent node list reference """
            next_node = None
            for i, _ in enumerate(node_list):
                k = i + 1 if i is not len(node_list) - 1 else len(node_list) - 1
                if node_list[i] == cur_node:
                    next_node = node_list[k]
                    break

            assert next_node is not None, f'Invalid node list / node: {node_list} {cur_node}'

            return next_node

        def _fff_gen_node_imp(entry, cur, lev=1):

            indent = " " * lev * 4

            (folder_cnt, file_cnt, disk_cnt,
             ordered_node_list) = _get_summmary(cur)

            sum_str = ""
            if folder_cnt > 0:
                sum_str += str(folder_cnt) + " folders "
            if file_cnt > 0:
                sum_str += str(file_cnt) + " files "
            if disk_cnt > 0:
                sum_str += str(disk_cnt) + " disks "

            self.fff_write(f'{indent}/* {sum_str} */')

            for typ, val in cur:

                if typ == FffEltType.FOLDER:
                    (node, folder, key, path) = val

                    next_folder = _get_next_node(ordered_node_list, node)
                    alias_list = [
                        '"' + re.escape(i) + '"' for i in folder.alias
                    ]
                    alias_list = ', '.join(alias_list)

                    if entry.get(key):

                        self.fff_write(
                            f'{indent}fff_folder({node}, {path}, {next_folder},'
                        )

                        self.fff_write(
                            f'{indent}\tfff_folder_path({alias_list}),')

                        _fff_gen_node_imp(entry, entry[key], lev + 1)
                        self.fff_write(
                            f'{indent}), /* fff_folder (lvl {lev}) */')

                    else:
                        # empty folder, insert dummy file
                        assert FffYmltoCFsConv.Defaults.FFF_DUMMY_INS_EMPTY_FOLDER is True, \
                            "only dummy node insertion supported in this version"
                        if FffYmltoCFsConv.Defaults.FFF_DUMMY_INS_EMPTY_FOLDER:
                            self.fff_write(
                                f'{indent}fff_folder({node}, {path}, {next_folder}, /* empty */'
                            )
                            self.fff_write(
                                f'{indent}\tfff_folder_path({alias_list}),')
                            self.fff_write(f'{indent}\tfff_mem_file(dummy_node, {path}.{node}, dummy_node,')
                            self.fff_write(f'{indent}\tfff_path("dummy"), fff_access(FFF_NO_ACCESS),')
                            self.fff_write(f'{indent}\t),')
                            self.fff_write(f'{indent}),')


                elif typ == FffEltType.FILE:
                    (node, file, path) = val

                    next_node = _get_next_node(ordered_node_list, node)

                    file_basename = os.path.basename(file.path)
                    self.fff_write(
                        f'{indent}  fff_mem_file({node}, {path}, {next_node },')
                    self.fff_write(f'{indent}    fff_path("{file_basename}"),')
                    self.fff_write(f'{indent}    fff_access({file.access}),')

                    if file.inc_bin_ref != 'NONE':
                        incbin = incbin_prefix(file.inc_bin_ref)
                        rom_data = f'{incbin}_data'
                        rom_size = f'{incbin}_size'
                        self.fff_write(
                            f'{indent}    fff_mem_content({rom_data}, {rom_size}),'
                        )

                    elif file.array is not None:
                        rom_data = file.array
                        rom_size = file.size
                        self.fff_write(
                            f'{indent}    fff_mem_buffer({rom_data}, {rom_size}),'
                        )

                    self.fff_write(f'{indent}  ),')


                elif typ == FffEltType.DISK:
                    (node, key, drive_path, path) = val

                    next_node = _get_next_node(ordered_node_list, node)
                    if entry.get(key):
                        self.fff_write(
                            f'{indent}fff_disk({node}, {path}, {next_node},')
                        self.fff_write(
                            f'{indent}  fff_disk_path("{drive_path}"),')
                        _fff_gen_node_imp(entry, entry[key], lev + 1)
                        self.fff_write(f'{indent}), /* fff_disk */')


        root = self.fff_node_hierarchy
        cur = next(iter(((root).items())))

        self.fff_write(f'\n\nimp_fff({self.drive_name},')

        _fff_gen_node_imp(root, cur[1], 1)

        self.fff_write('); /*def_fff */\n\n')


def main(argv):

    parser = ArgumentParser(
        description="fff yml to c filesystem converter (v1.0.1)")
    parser.add_argument(
        '--cfg',
        '-c',
        type=str,
        help='FFF input yaml configuration',
        required=True)
    parser.add_argument(
        '-o',
        nargs='?',
        type=str,
        required=False,
        help="output C file filesystem")
    parser.add_argument('-d', action='store_true', help="Debug Mode")
    args = parser.parse_args()

    outputfile = args.o
    if outputfile is None or outputfile == "":
        outputfile = "__fff_user_def_file_framework.c"

    cinit()

    print(f'Generating {outputfile}...')

    with open(
            args.cfg, encoding="utf-8") as yml_input, open(
                outputfile, "w", encoding="utf-8") as c_output:

        full_tree = safe_load(yml_input)

        tree = FffYmltoCFsConv(full_tree, c_output)

        tree.fff_prolog(args.cfg)

        tree.fff_yml_2_tree(full_tree)

        tree.fff_gen_root_pwd()

        tree.fff_gen_incbin()

        tree.fff_gen_array_section()

        tree.fff_gen_def()

        tree.fff_gen_imp()

        tree.fff_gen_cmdline()

        tree.fff_epilog()

        print('done !')


if __name__ == '__main__':
    main(sys.argv[1:])
