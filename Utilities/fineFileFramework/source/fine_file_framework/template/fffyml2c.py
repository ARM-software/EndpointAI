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
import yaml
import argparse
import os
import time
import re
import operator
from collections import defaultdict



class FffHostDirAttr:

    class FffHostDirFilter:
        ''' File Filtering for Host mapped folders '''
        def __init__(self, val):
            self._size = None
            self._name = None

            #for i in val:
            #    if isinstance(i, dict):
            #        (k, v) =  (list(i.items())[0][0], list(i.items())[0][1])

            #        self._size = v.split() if k == 'Size-Filter' else self._size
            #       self._name = v.split() if k == 'Name-Filter' else self._name


            key_to_attr = {'Size-Filter': '_size', 'Name-Filter': '_name'}

            for i in val:
                if isinstance(i, dict):
                    k, v = list(i.items())[0]
                    setattr(self, key_to_attr.get(k), v.split())

            # filter comparison operator + combination
            self.ops = {
                "<":    operator.lt,
                "<=":   operator.le,
                "==":   operator.eq,
                ">":    operator.gt,
                ">=":   operator.ge,
                "AND":  operator.and_,
                "OR":   operator.or_}


            #if self._size:
            #    assert (self._size[0] == 'AND' or self._size[0] == 'OR'), f'invalid size filter {self._size}'

            #if self._name:
            #    assert (self._name[0] == 'AND' or self._name[0] == 'OR'), f'invalid name filter {self._name}'

            assert self._size is None or self._size[0] in ['AND', 'OR'], f'invalid size filter {self._size}'
            assert self._name is None or self._name[0] in ['AND', 'OR'], f'invalid name filter {self._name}'

            # parse size filter
            self._filter_size = 0
            self.size_cmp = None
            if self._size:
                cmpre = '"?(?P<cmp>([<>]=?|==))(?P<size>((\d+(\.\d+)?)|(\.\d+)))(?P<unit>[KM]?)"?'
                cmpre = re.compile(cmpre)
                size_compare = cmpre.match(self._size[1])
                assert size_compare, f'invalid size filter {self._size}'

                # scale based on 'K' KByte (default) or 'M' MByte
                scale = 1024*1024 if size_compare.group('unit') == 'M' else 1024

                self._filter_size = int(scale * float(size_compare.group('size')))
                self.size_cmp = self.ops[size_compare.group('cmp')]

            # parse name
            if self._name:
                self._name_compare = re.compile(self._name[1])



        def __size_cmp__(self, size):
            """ compare filesize with filter size       """

            return self.size_cmp(size, self._filter_size)


        def condition_match(self, filename):
            if not os.path.exists(filename):
                return False

            filesize = os.path.getsize(filename)
            filename = os.path.basename(filename)
            extension = filename.split(".")[-1]


            match_size = True if self.__size_cmp__(filesize) and self._size else False

            match_name = True if self._name_compare.match(filename) and self._name else False

            match = True

            if self._size:
                match = self.ops[self._size[0]](match, match_size)

            if self._name:
                match = self.ops[self._name[0]](match, match_name)

            return match


        def __repr__(self):
            return f"Filter: size={self._size} name={self._name}"


    def __init__(self, val):

        assert isinstance(val, dict), "FffHostDirAttr expect an input dictionnary !!"

        self._root = val.get('Source')
        self._recursive = True if val.get('Recursive') == "YES" else False

        access = val.get('Access')
        self._access = "FFF_" + \
            str(access).replace(" ", "_") if access != None else FffTree.Defaults.FFF_HOST_FOLDER_ACCESS

        self._alias= []
        path_list =  val.get('Path')
        # aliases optional
        if len(path_list) > 0:
            try:
                assert isinstance(path_list[0], dict), "Folder is not a dict, YML to be fixed !!"
            except AssertionError as msg:
                print(msg)
                print(f'offending entry : {val}')
                exit(1)

            self._alias= [ i.get('Alias') for i in path_list ]

        self._filter = val.get('Filters')
        if self._filter:
            self._filter = FffHostDirAttr.FffHostDirFilter(self._filter)


        self._exclusion_list = []
        exclusion_list =  val.get('Exclude-Files')
        # exclusion_list optional
        if len(exclusion_list) > 0:
            try:
                assert isinstance(exclusion_list[0], dict), "Folder is not a dict, YML to be fixed !!"
            except AssertionError as msg:
                print(msg)
                print(f'offending entry : {val}')
                exit(1)

            # expect full path
            self._exclusion_list = [ i.get('File') for i in exclusion_list ]


    def __repr__(self):
        return f"FFFHostDir: root={self._root} recurs={self._recursive} access={self._access} alias={self._alias} exc={self._exclusion_list}"



class FffHostFileTree:
    """ Host directory tree """
    def __init__(self, path, recursive, isdir='True'):

        self._name = path
        self._isdir = isdir
        self._children = []
        if os.path.isdir(path):
            for child_name in os.listdir(path):
                child_path = os.path.join(path, child_name)
                child_is_dir = os.path.isdir(child_path)
                if recursive or not child_is_dir:
                    self._children.append(FffHostFileTree(
                        child_path, recursive, child_is_dir))


    def __repr__(self):
        if self._isdir:
            return f"Directory {self._name}"
        return f"File {self._name}"




class FFFFolderAttrib:
       def __init__(self, alias, node, access):

            self._node = node
            self._alias= alias

            if access != None and 'FFF' in access:
                self._access = access
            else:
                self._access = "FFF_" + \
                    str(access).replace(
                        " ", "_") if access != None else FffTree.Defaults.FFF_ACCESS
            assert self._access in ['FFF_READ_ONLY', 'FFF_READ_WRITE'], f'invalid access {self._access}'
            self._file_list = []

       def __repr__(self):
           nb_files = len(self._file_list)

           if self._alias!= "":
               return f"folder {self._alias} {self._alias} {self._node} {self._access} {nb_files} files : {self._file_list}"
           return f"no folder {nb_files} files {self._file_list} "


class FFFFileAttrib:
       def __init__(self, path, alias,   access, incbin, size_arr_section=(None, None, None)):
            self.path = path
            self._alias= alias
            if access != None and 'FFF' in access:
                self._access = access
            else:
                self._access = "FFF_"+str(access).replace(" ", "_") if access != None else FffTree.Defaults.FFF_ACCESS
            assert self._access in ['FFF_READ_ONLY', 'FFF_READ_WRITE'], f'invalid access {self._access}'
            size, array, section = size_arr_section
            self.section = section if section != "" else FffTree.Defaults.FFF_SECTION
            self.array = array
            self.size = size
            self.incBinRef = incbin if incbin != None else "NONE"

       def __repr__(self):
            return f"file | {self.path} | {self._alias} |   {self._access} |  {self.incBinRef} | {self.section} | {self.size}"


class FffTree:
    class Defaults:
        """ FFF defaults behaviour  """
        FFF_DRIVE_NAME = "Local_Disk"
        FFF_ACCESS = "FFF_READ_WRITE"
        FFF_HOST_FOLDER_ACCESS = "FFF_READ_WRITE"
        FFF_HOST_FILE_ACCESS = "FFF_READ_ONLY"
        FFF_DUMMY_INS_EMPTY_FOLDER = True
        FFF_SECTION = ".data"
        FFF_FILE_SECTION = ".data"

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

        def __init__(self):
             self._total_source_files = 0
             self._total_static_mem_files = 0
             self._total_heap_mem_files = 0
             self._total_source_files_mem = 0
             self._total_static_mem_files_mem = 0
             self._source_will_discard = False

        def __str__(self):
             return f'''
        /*
            ~~~~~~ FFF Statistics ~~~~~~~

            Total source files          = {self._total_source_files}
            Total source files memory   = {self._total_source_files_mem} bytes
            Total static memory files   = {self._total_static_mem_files}
            Total static memory         = {self._total_static_mem_files_mem} bytes
            Total files using heap      = {self._total_heap_mem_files}

        */'''


    def __init__(self, yaml_tree,     output):

        self._yaml_tree = yaml_tree

        self._drive_name = FffTree.Defaults.FFF_DRIVE_NAME

        self._file_section = FffTree.Defaults.FFF_FILE_SECTION

        self._file_section_size = None

        self._arglist = []
        self._drive_list = []
        self._folder_node_list = defaultdict(list)
        self._host_tree_list = {}

        self._folder_tree = {}

        self._inc_bin_list = []
        self._all_file_list = set()
        self._mem_size_section_list = []

        self.output = output

        # top level items parsing
        self._fff_parse_toplvl()

        self._stats = FffTree.FffStats()


    @property
    def drive_name(self):
        return self._drive_name

    @drive_name.setter
    def drive_name(self, value):
        if value != '':
            self._drive_name = value

    def fff_write(self, string):
        print(string, file=self.output)



    def _is_host_dir(self, val):
        """ when folder has a source, we're dealing with host folder"""
        return val.get('Source') != None


    def _fff_parse_toplvl(self):

        # drive name, file section/size limit extraction
        root= self._yaml_tree.get('root')
        for i in root:
            if isinstance(i, dict):

                # handle section / max size for sources
                mem = i.get('Memory')
                if mem:
                    for memlist in mem:
                        filesection = memlist.get('Section')
                        if filesection:
                            self._file_section = filesection

                        file_section_size = memlist.get('Size')
                        if file_section_size:
                            szre = '"?(?P<size>((\d+(\.\d+)?)|(\.\d+)))(?P<unit>[KM]?)"?'
                            szre = re.compile(szre)
                            size = szre.match(file_section_size)
                            assert size, f'invalid section size  {file_section_size}'

                            # scale based on 'K' KByte (default) or 'M' MByte
                            scale = 1024*1024 if size.group('unit') == 'M' else 1024
                            self._file_section_size = int(scale * float(size.group('size')))

                # fff drive name
                name = i.get('Name')
                if name:
                    self.drive_name = name


        # CMD Line arg list
        cmdline= self._yaml_tree.get('Command-line')
        for i in cmdline:
            if isinstance(i, dict):
                arg = i.get('Arg')
                if arg:
                    self._arglist.append(arg)



    def _alias_to_node(self, alias):
        """ convert alias into lower case and replace slashes with underscores """
        return alias.lower().replace("/", "_").replace('\\', '_').replace(".", "_")


    def _fff_scan_host_folder(self, cur_dir, host_dir_attr, parent,  indent=0):

        if cur_dir._isdir:

            # for host, folder aliases & nodes are created from full path folder names
            alias = cur_dir._name
            node = self._alias_to_node(alias)
            folder_struct_path = f'{parent}.{node}'


            folder_alias = [alias]
            # if root directiory has aliases, append them
            if cur_dir._name == host_dir_attr._root:
                folder_alias.extend(host_dir_attr._alias)

            folder = FFFFolderAttrib(folder_alias , node, host_dir_attr._access)

            self._folder_tree[folder_struct_path] = folder
            self._folder_node_list[folder_struct_path].append(node)


        for child in cur_dir._children:

            if child._isdir:
                # scan one level down
                self._fff_scan_host_folder(child, host_dir_attr, folder_struct_path, indent + 1)

            else:

                folder_node = self._alias_to_node(cur_dir._name)
                # nested struct
                folder_struct_path = f'{parent}.{folder_node}'

                cur_file = child._name

                # filtering checks
                if cur_file not in host_dir_attr._exclusion_list and \
                    host_dir_attr._filter.condition_match(cur_file) and \
                    not self._stats._source_will_discard :

                    sz = os.path.getsize(cur_file)
                    if self._stats._total_source_files_mem + sz > self._file_section_size:
                        print(f'Total source size limit reached, discard subsequent source files')
                        self._stats._source_will_discard = True
                        continue

                    if sz== 0:
                        print(f'Source file {cur_file} size = 0 skipping...')
                        continue


                    self._stats._total_source_files +=1
                    self._stats._total_source_files_mem +=sz

                    path_to_file = cur_file
                    file_node = self._alias_to_node(path_to_file)

                    assert path_to_file != None, "path error"
                    self._inc_bin_list.append(path_to_file)

                    fs_attr = "r"
                    if 'WRITE' in val.get('Access'):
                        fs_attr = "w"
                    self._all_file_list.add((path_to_file, fs_attr))

                    file = FFFFileAttrib(
                        path_to_file, file_node, host_dir_attr._access, path_to_file)

                    self._folder_tree[folder_struct_path]._file_list.append(file)



    def _fff_gen_def_host_directory(self, cur_dir, parent, indent=0):

        if cur_dir._isdir:

            node = self._alias_to_node(cur_dir._name)
            path = f'{parent}.{node}'

            folder = self._folder_tree.get(path)
            if folder:
                node = folder._node
                self.fff_write(f'\tuse_fff_folder({node},')
                for file in folder._file_list:
                    self.fff_write(f'\t\tarm_mem_file_node_t\t{file._alias};')

                # TOCHECK is this needed ? should dummy node be unique ?
                if FffTree.Defaults.FFF_DUMMY_INS_EMPTY_FOLDER and len(folder._file_list) == 0:
                    self.fff_write(f'\t\tarm_mem_file_node_t\tdummy_node;')


        for child in cur_dir._children:
            if child._isdir:
                self._fff_gen_def_host_directory(child, path, indent + 1)

                self.fff_write(f'\t)')



    def _fff_gen_imp_host_directory(self, cur_dir, parent, indent=0):

        if cur_dir._isdir:

            alias = cur_dir._name
            node = alias.replace("/", "_").replace('\\', '_')
            sub = f'.{node}'

            path = f'{parent}{sub}'

            folder = self._folder_tree.get(f'{path}')
            if folder:

                node = folder._node

                # serch for next
                next = None
                dir = self._folder_node_list.get(path)

                for i, item in enumerate(dir):
                    k = i + 1 if i != len(dir) - 1 else len(dir) - 1
                    if dir[i] == node:
                        next = dir[k]
                        break

                if next == None:
                    print('invalid folder')
                    exit(1)



                self.fff_write(f'\tfff_folder({node}, {parent}{sub}, {next},')
                # insert alias list
                alias_list =[f'"{i}"' for i in folder._alias]
                alias_list = ',\n\t\t\t\t'.join(alias_list)
                self.fff_write(f'\t\tfff_folder_path({alias_list}),')
                self.fff_write(f'\t\tfff_access({folder._access}),')



                if len(folder._file_list) >= 0:
                    self.fff_write(f'\tfff_list(')
                    for i, item in enumerate(folder._file_list):
                        k = i + 1 if i != len(folder._file_list) - \
                                              1 else len(folder._file_list) - 1
                        cur_file = folder._file_list[i]
                        next_file = folder._file_list[k]

                        self.fff_write(
                            f'''\t\tfff_mem_file({cur_file._alias}, {parent}{sub}, {next_file._alias},
    fff_path("{cur_file.path}"),
    fff_access({cur_file._access}),''')
                        incbin = cur_file.incBinRef.upper().replace("/", "_").replace('\\', '_').replace(".", "_")
                        rom_data = f'{incbin}_data'
                        rom_size = f'{incbin}_size'

                        if cur_file.incBinRef != 'NONE':
                            self.fff_write(f'\t\t\tfff_mem_content({rom_data}, {rom_size}),')
                        self.fff_write(f'\t\t),')
                    self.fff_write(f'\t),')

        for child in cur_dir._children:

            if child._isdir:

                self._fff_gen_imp_host_directory(child, path, indent + 1)

                self.fff_write(f'\t)')


    def fff_yml_2_tree(self, data, path=''):

        if isinstance(data, dict):

            for key, val in data.items():
                sub = ''

                if key == 'root':
                    sub = f'&{self.drive_name}'

                elif key == 'Disk':
                    disk_path = val['Path']
                    disk_drive = disk_path.lower().replace(":", "")
                    sub = f'.disk_{disk_drive}'
                    self._drive_list.append((disk_drive, disk_path))

                elif key == 'Folder':

                    # host folder
                    if self._is_host_dir(val):
                        dir_top = val.get('Source')

                        if not os.path.isdir(dir_top):
                            print(f'Host directory {dir_top} not found')
                            return


                        # set host dir attributes + filtering
                        host_dir_attr = FffHostDirAttr(val)

                        # build host tree
                        local_dir = FffHostFileTree(dir_top, host_dir_attr._recursive)

                        # attach tree
                        self._host_tree_list[f'{path}'] = local_dir

                        # scan files, retrieve inbin ref + filtering
                        self._fff_scan_host_folder(local_dir, host_dir_attr, f'{path}')

                        return

                    else:

                        cur_folder = val
                        path_list = cur_folder.get('Path')
                        assert len(path_list) > 0, "!! Folder path without 'Path' entry, YML to be fixed !!"

                        try:
                            assert isinstance(path_list[0], dict), "Folder is not a dict, YML to be fixed !!"
                        except AssertionError as msg:
                            print(msg)
                            print(f'offending entry : {val}')
                            exit(1)

                        # use 1st alias to derive node name
                        alias_list = [ i.get('Alias') for i in path_list ]
                        alias = alias_list[0]
                        assert alias != "",  "!! Folder path without 'Alias' entry, YML to be fixed !!"

                        node = alias.replace('/', '_').replace('\\', '_')
                        sub = f'.{node}'


                        folder_access = cur_folder.get('Access')

                        folder = FFFFolderAttrib(
                            alias_list, node, folder_access)
                        self._folder_tree[f'{path}{sub}'] = folder

                        self._folder_node_list[f'{path}'].append(node)

                elif key == 'File':

                    path_to_file = val['Path']
                    node_ref = path_to_file.lower().replace("/", "_").replace('\\', '_').replace(".", "_")



                    incBinRef = None
                    source_ref = val.get('Source')
                    if source_ref:

                        if  os.path.exists(source_ref):
                            sz = os.path.getsize(source_ref)
                            if self._stats._total_source_files_mem + sz > self._file_section_size:
                                print(f'Total source size limit reached, discard subsequent source files')
                                self._stats._source_will_discard = True
                                continue
                            if os.path.getsize(source_ref) == 0:
                                print(f'Source file {source_ref} size = 0 skipping...')
                                continue
                        else:
                            print(f'Source file {source_ref} does not exist, skipping...')
                            continue


                        self._stats._total_source_files +=1
                        self._stats._total_source_files_mem +=sz

                        self._inc_bin_list.append(source_ref)
                        incBinRef = source_ref



                    mem_ref = val.get('Memory')
                    size_name, array_name, mem_section = (None, None, None)
                    if mem_ref:
                        mem_size = mem_ref.get('Size')
                        mem_section = mem_ref.get('Section')
                        assert mem_size != None, f'Invalid Memory entry, no size {mem_ref}'
                        assert mem_section != None, f'Invalid Memory entry, no section {mem_ref}'

                        size_cst = node_ref.upper().replace("/", "_").replace('\\', '_').replace(".", "_")+'_SZ'
                        size_name = node_ref.lower().replace("/", "_").replace('\\', '_').replace(".", "_")+'_size'
                        array_name = node_ref.lower().replace("/", "_").replace('\\', '_').replace(".", "_")+'_arr'

                        self._stats._total_static_mem_files_mem +=mem_size

                        self._mem_size_section_list.append(
                            (size_cst, size_name, array_name, mem_size, mem_section))

                    if not incBinRef:
                        if mem_ref:
                            self._stats._total_static_mem_files +=1
                        else:
                            self._stats._total_heap_mem_files +=1

                    fs_attr = "r"
                    if 'WRITE' in val.get('Access'):
                        fs_attr = "w"
                    self._all_file_list.add((path_to_file, fs_attr))


                    file = FFFFileAttrib(path_to_file, node_ref,
                                      val.get('Access'), incBinRef,
                                      (size_name, array_name, mem_section))

                    try:
                        self._folder_tree[path]._file_list.append(file)
                    except KeyError:
                        # file without parent dir, attach to upper level (root or drive)
                        folder = FFFFolderAttrib('', '', None)
                        self._folder_tree[path] = folder
                        self._folder_tree[path]._file_list.append(file)

                # continue next level scan
                self.fff_yml_2_tree(val, f'{path}{sub}')

        elif isinstance(data, list):

            for item in data:
                self.fff_yml_2_tree(item, f'{path}')


    def fff_gen_incbin(self):

        """Generate incbin aprt using source reference + tag"""

        srcList = self._inc_bin_list

        if not srcList:
            return

        self.fff_write(FffTree.Defaults.FFF_INCBIN_PART_HDR.format(self._file_section))

        inc_bin_list = zip([v.upper().replace("/", "_").replace('\\', '_').replace(".", "_")
                         for v in srcList], srcList)

        for tag, src in inc_bin_list:
            self.fff_write(f'INCBIN({tag}, "{src}");')

        self.fff_write("""

    /* end of incbin part */

        """)

    def fff_gen_allfile_list(self):

        """Generate incbin aprt using source reference + tag"""

        srcList = self._all_file_list

        if not srcList:
            return


        self.fff_write(f'char * {self._drive_name}_filelist[] = {{')
        for src, attr in srcList:
            self.fff_write(f'\t"{src}","{attr}",')

        self.fff_write("\tNULL,\n};\n\n")




    def fff_gen_array_section(self):
        """Generate incbin aprt using source reference + tag"""

        self.fff_write('''

    /* static arrays with section definition */

    ''')

        for (size_cst, size_name, array_name, mem_size, mem_section) in self._mem_size_section_list:

            self.fff_write(f'#define {size_cst}\t({mem_size})')
            self.fff_write(f'const uint32_t {size_name}= {size_cst};')
            self.fff_write(f'__attribute__ ((section("{mem_section}")))  uint8_t {array_name}[{size_cst}];\n\n')

        self.fff_write("""

    /* end of static arrays with section definition part */

        """)



    def fff_gen_cmdline(self):
        """Generate incbin aprt using source reference + tag"""

        self.fff_write(FffTree.Defaults.FFF_CMDLINE_PART_HDR)

        arglist = " ".join(self._arglist)
        self.fff_write(f'\treturn "./app {arglist}";')

        self.fff_write("""}\n\n""")


    def fff_prolog(self, yml):

        self.fff_write(FffTree.Defaults.FFF_C_HDR.
            format(time.asctime(time.localtime(time.time())), yml))


    def fff_epilog(self):

        self.fff_write(self._stats)

        self.fff_write("\n#endif /* __FFF_USER_DEFINE_FILE_FRAMEWORK__ */")


    def fff_gen_def(self, data, path='', level=0):
        """
        """

        if isinstance(data, dict):
            for key, val in data.items():

                # indent = " " * level*4
                indent = ''
                sub = ''

                if key == 'root':
                    self.fff_write(f'{indent}def_fff({self.drive_name},')
                    sub = f'&{self.drive_name}'

                    # files without drive, without folder
                    files = self._folder_tree.get(sub)
                    if files:
                        for file in files._file_list:
                            self.fff_write(f'\t{indent}arm_mem_file_node_t\t{file._alias};')

                elif key == 'Disk':

                    drive = val['Path'].lower().replace(":", "")
                    sub = f'.disk_{drive}'
                    self.fff_write(f'{indent} use_fff_disk(disk_{drive},')

                    folder = self._folder_tree.get(f'{path}{sub}')
                    if folder:
                        for file in folder._file_list:
                            self.fff_write(f'\t{indent}\tarm_mem_file_node_t\t{file._alias};')

                elif key == 'Folder':
                    if self._is_host_dir(val):
                        local_dir = self._host_tree_list.get(f'{path}')
                        if local_dir:
                            self._fff_gen_def_host_directory(local_dir, f'{path}')

                    else:

                        cur_folder = val
                        path_list = cur_folder.get('Path')
                        alias = path_list[0].get('Alias')
                        alias = alias.replace("/", "_").replace('\\', '_')

                        sub = f'.{alias}'

                        folder = self._folder_tree.get(f'{path}{sub}')
                        if folder:
                            node = folder._node
                            self.fff_write(f'\t{indent}use_fff_folder({node},')
                            for file in folder._file_list:
                                self.fff_write(f'\t{indent}\tarm_mem_file_node_t\t{file._alias};')

                            # TOCHECK is this needed ? should dummy node be unique ?
                            if FffTree.Defaults.FFF_DUMMY_INS_EMPTY_FOLDER and len(folder._file_list) == 0:
                                self.fff_write(f'\t{indent}\tarm_mem_file_node_t\tdummy_node;')


                self.fff_gen_def(val, f'{path}{sub}', level + 1)

                if key == 'Folder' and isinstance(val, str):
                    # skip braces closing host folders
                    pass

                # close braces
                elif key in {'root', 'Disk', 'Folder'}:
                    self.fff_write(f'\t{indent});')

        elif isinstance(data, list):

            for item in data:
                self.fff_gen_def(item, f'{path}', level + 1)


    def fff_gen_imp(self, data, path='', level=0):
        """
        """

        if isinstance(data, dict):

            for key, val in data.items():

                # indent = " " * level*4
                indent = ''
                sub = ''

                if key == 'root':
                    self.fff_write(f'{indent}imp_fff({self.drive_name},')
                    sub = f'&{self.drive_name}'

                    # files without drive, without folder
                    files = self._folder_tree.get(sub)
                    if files:
                        self.fff_write(f'\t{indent}fff_list(')
                        for i, item in enumerate(files._file_list):
                            k = i + 1 if i != len(files._file_list) - \
                                              1 else len(files._file_list) - 1

                            cur_file = files._file_list[i]
                            next_file = files._file_list[k]
                            self.fff_write(
                                f'''\t{indent}\tfff_mem_file({cur_file._alias}, {sub}, {next_file._alias},
    fff_path("{cur_file.path}"),
    fff_access({cur_file._access}),''')
                            incbin = cur_file.incBinRef.upper().replace("/", "_").replace('\\', '_').replace(".", "_")
                            rom_data = f'{incbin}_data'
                            rom_size = f'{incbin}_size'

                            if cur_file.incBinRef != 'NONE':
                                self.fff_write(f'\t{indent}\t\tfff_mem_content({rom_data}, {rom_size}),')


                            if cur_file.array != None:
                                 rom_data = cur_file.array
                                 rom_size = cur_file.size
                                 self.fff_write(f'\t{indent}\t\tfff_mem_buffer({rom_data}, {rom_size}),')

                            self.fff_write(f'\t{indent}\t),')
                        self.fff_write(f'\t{indent}),')

                elif key == 'Disk':
                    drive = val['Path'].lower().replace(":", "")
                    sub = f'.disk_{drive}'

                    next = 'xxx'
                    for i, item in enumerate(self._drive_list):
                        k = i + 1 if i != len(self._drive_list) - \
                                              1 else len(self._drive_list) - 1
                        (cur_drive, cur_path) = self._drive_list[i]
                        if cur_drive == drive:
                            (next_drive, next_path) = self._drive_list[k]
                            # next = next_drive
                            break
                    self.fff_write(f'{indent} fff_disk(disk_{drive}, {path}, disk_{next_drive},')
                    self.fff_write(f'{indent}\tfff_disk_path("{cur_path}"),')


                    folder = self._folder_tree.get(f'{path}{sub}')

                    if folder:

                        self.fff_write(f'\t{indent}fff_list(')
                        for i, item in enumerate(folder._file_list):
                            k = i + 1 if i != len(folder._file_list) - 1 else len(folder._file_list) - 1
                            cur_file = folder._file_list[i]
                            next_file = folder._file_list[k]

                            self.fff_write(
                                f'''\t{indent}\tfff_mem_file({cur_file._alias}, {path}{sub}, {next_file._alias},
fff_path("{cur_file.path}"),
fff_access({cur_file._access}),''')
                            incbin = cur_file.incBinRef.upper().replace("/", "_").replace('\\', '_').replace(".", "_")


                            #rom_data = cur_file.array
                            #rom_size = cur_file.size

                            if cur_file.incBinRef != 'NONE':

                                rom_data = f'{incbin}_data'
                                rom_size = f'{incbin}_size'

                                self.fff_write(f'\t{indent}\t\tfff_mem_content({rom_data}, {rom_size}),')

                            if cur_file.array != None:
                                 rom_data = cur_file.array
                                 rom_size = cur_file.size
                                 self.fff_write(f'\t{indent}\t\tfff_mem_buffer({rom_data}, {rom_size}),')


                            self.fff_write(f'\t{indent}\t),')
                        self.fff_write(f'\t{indent}),')




                elif key == 'Folder':
                    if self._is_host_dir(val):
                        # Host directory
                        local_dir = self._host_tree_list.get(f'{path}')
                        if local_dir:
                            self._fff_gen_imp_host_directory(local_dir, f'{path}')

                    else:

                        cur_folder = val
                        path_list = cur_folder.get('Path')
                        alias = path_list[0].get('Alias')
                        alias = alias.replace("/", "_").replace('\\', '_')

                        #alias = val['Alias'].replace("/", "_")
                        sub = f'.{alias}'

                        folder = self._folder_tree.get(f'{path}{sub}')
                        if folder:

                            node = folder._node

                            # serch for next
                            next = None
                            dir = self._folder_node_list.get(path)
                            for i, item in enumerate(dir):
                                k = i + 1 if i != len(dir) - \
                                                      1 else len(dir) - 1
                                if dir[i] == node:
                                    next = dir[k]
                                    break

                            assert next != None, f'Invalid folder list : {dir}'


                            self.fff_write(f'\t{indent}fff_folder({node}, {path}, {next},')
                            # insert alias list
                            alias_list =[f'"{i}"' for i in folder._alias]
                            alias_list = ',\n\t\t\t\t'.join(alias_list)
                            self.fff_write(f'\t{indent}\tfff_folder_path({alias_list}),')
                            self.fff_write(f'\t{indent}\tfff_access({folder._access}),')

                            if len(folder._file_list) >= 0:
                                self.fff_write(f'\t{indent}fff_list(')
                                for i, item in enumerate(folder._file_list):
                                    k = i + \
                                        1 if i != len(
                                            folder._file_list) - 1 else len(folder._file_list) - 1
                                    cur_file = folder._file_list[i]
                                    next_file = folder._file_list[k]

                                    self.fff_write(
                                        f'''\t{indent}\tfff_mem_file({cur_file._alias}, {path}{sub}, {next_file._alias},
    fff_path("{cur_file.path}"),
    fff_access({cur_file._access}),''')
                                    incbin = cur_file.incBinRef.upper().replace("/", "_").replace('\\', '_').replace(".", "_")


                                    #rom_data = cur_file.array
                                    #rom_size = cur_file.size

                                    if cur_file.incBinRef != 'NONE':

                                        rom_data = f'{incbin}_data'
                                        rom_size = f'{incbin}_size'

                                        self.fff_write(f'\t{indent}\t\tfff_mem_content({rom_data}, {rom_size}),')

                                    if cur_file.array != None:
                                         rom_data = cur_file.array
                                         rom_size = cur_file.size
                                         self.fff_write(f'\t{indent}\t\tfff_mem_buffer({rom_data}, {rom_size}),')


                                    self.fff_write(f'\t{indent}\t),')
                                self.fff_write(f'\t{indent}),')

                            # TOCHECK is this needed ? should dummy node be unique ?
                            else:
                                if FffTree.Defaults.FFF_DUMMY_INS_EMPTY_FOLDER:
                                    self.fff_write(f'\t{indent}\tfff_mem_file(dummy_node, {path}{sub}, dummy_node,fff_path("dummy"), fff_access(FFF_READ_WRITE),')

                self.fff_gen_imp(val, f'{path}{sub}', level + 1)

                # close brackets
                if key in {'root', 'Disk', 'Folder'}:
                    self.fff_write(f'\t{indent});')

        elif isinstance(data, list):

            for item in data:
                self.fff_gen_imp(item, f'{path}', level + 1)







def main(argv):


    parser = argparse.ArgumentParser(description="Fine File Framework YMAL parser  (v1.0.0)")
    parser.add_argument('--cfg', type=str, help='input yaml', default='sample.yml', required=False)
    parser.add_argument('-o', nargs='?', type= str,  required=False, help="output C file filesystem")
    parser.add_argument('-d', action='store_true',  help="Debug Mode")
    args = parser.parse_args()

    outputfile = args.o
    if outputfile == None or outputfile == "":
        outputfile = "__fff_user_def_file_framework.c"



    with open(args.cfg) as input, open(outputfile, "w") as output:

        full_tree = yaml.safe_load(input)

        tree = FffTree(full_tree, output)

        tree.fff_prolog(args.cfg)


        tree.fff_yml_2_tree(full_tree)


        tree.fff_gen_incbin()


        tree.fff_gen_array_section()

        tree.fff_gen_def(full_tree)

        print("\n\n", file=output)
        tree.fff_gen_imp(full_tree)


        # command line
        tree.fff_gen_cmdline()

        if args.d:
            tree.fff_gen_allfile_list()

        tree.fff_epilog()



        print('done !')


if __name__ == '__main__':
    main(sys.argv[1:])
