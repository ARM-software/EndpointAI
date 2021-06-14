import os
import re
import colorama
from colorama import init,Fore, Back, Style
import argparse
import subprocess

init()

parser = argparse.ArgumentParser(description='Run tests')
parser.add_argument('-o', nargs='?',type = str, default="log.html",help="Log")
parser.add_argument('-a', nargs='?',type = str, default=None,help="After test")
parser.add_argument('-t', action='store_true', help="Dry run")
parser.add_argument('-c', action='store_true', help="Clean")

parser.add_argument('others', nargs=argparse.REMAINDER)

args = parser.parse_args()

#EIGEN_TEST_CXX11 not set
disabled=["initializer_list_construction","diagonal_matrix_variadic_ctor"]

tests=[
"rand",
"meta",
"numext",
"sizeof",
"dynalloc",
"nomalloc",
"first_aligned",
"type_alias",
"nullary",
"mixingtypes",
"io",
"packetmath",
"unalignedassert",
"vectorization_logic",
"basicstuff",
"constructor",
"linearstructure",
"integer_types",
"unalignedcount",
"exceptions",
"redux",
"visitor",
"block",
"corners",
"symbolic_index",
"indexed_view",
"reshape",
"swap",
"resize",
"conservative_resize",
"product_small",
"product_large",
"product_extra",
"diagonalmatrices",
"adjoint",
"diagonal",
"miscmatrices",
"commainitializer",
"smallvectors",
"mapped_matrix",
"mapstride",
"mapstaticmethods",
"array_cwise",
"array_for_matrix",
"array_replicate",
"array_reverse",
"ref",
"is_same_dense",
"triangular",
"selfadjoint",
"product_selfadjoint",
"product_symm",
"product_syrk",
"product_trmv",
"product_trmm",
"product_trsolve",
"product_mmtr",
"product_notemporary",
"stable_norm",
"permutationmatrices",
"bandmatrix",
"cholesky",
"lu",
"determinant",
"inverse",
"qr",
"qr_colpivoting",
"qr_fullpivoting",
"upperbidiagonalization",
"hessenberg",
"schur_real",
"schur_complex",
"eigensolver_selfadjoint",
"eigensolver_generic",
"eigensolver_complex",
"real_qz",
"eigensolver_generalized_real",
"jacobi",
"jacobisvd",
"bdcsvd",
"householder",
"geo_orthomethods",
"geo_quaternion",
"geo_eulerangles",
"geo_parametrizedline",
"geo_alignedbox",
"geo_hyperplane",
"geo_transformations",
"geo_homogeneous",
"stdvector",
"stdvector_overload",
"stdlist",
"stdlist_overload",
"stddeque",
"stddeque_overload",
"sparse_basic",
"sparse_block",
"sparse_vector",
"sparse_product",
"sparse_ref",
"sparse_solvers",
"sparse_permutations",
"simplicial_cholesky",
"conjugate_gradient",
"incomplete_cholesky",
"bicgstab",
"lscg",
"sparselu",
"sparseqr",
"umeyama",
"nesting_ops",
"nestbyvalue",
"zerosized",
"dontalign",
"evaluators",
"sizeoverflow",
"prec_inverse_4x4",
"vectorwiseop",
"special_numbers",
"rvalue_types",
"dense_storage",
"ctorleak",
"mpl2only",
"inplace_decomposition",
"half_float",
"bfloat16_float",
"array_of_string",
"num_dimensions",
"stl_iterators",
"blasutil",
"initializer_list_construction",
"diagonal_matrix_variadic_ctor",
"fastmath"
]

header="""<html>
<head>
<title>Test Log</title>
<style>
h2 {
  color: #6666FF;
}
.ok {
  color: green;
}
.err {
  color: red;
}
h3 {
  background:  #CCCCCC;
}
</style>
</head>
<body>
<h1>Eigen test report on Cortex-M55</h1>
"""

footer="""</body>
</html>
"""

def ptest(m,o):
    print("<h2>%s</h2>"%m,file=o)
    print("\n"+Fore.GREEN + m + Style.RESET_ALL)

def psubtest(m,o):
    print("<h3>%s</h3>"%m,file=o)
    print("\n"+Fore.GREEN + m + Style.RESET_ALL)

def okmessage(m,o):
    print("<p class=\"ok\">%s</p>"%m,file=o)
    print("\n"+Fore.GREEN + m + Style.RESET_ALL)

def errmessage(m,o):
    print("<p class=\"err\">%s</p>"%m,file=o)
    print(Fore.RED + m + Style.RESET_ALL)

def printstd(s,o):
    for line in s.splitlines():
        print("  " + line.decode("utf-8"),file=o)


def runtest(test,o):
    cmd=["python3","/benchresults/regressionconfigs/run.py", "-p", "FVP", "-e", "build/%s" % test,  "m55"]
    print(cmd)
    completed=subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,timeout=3600)
    print("<h4>STDOUT</h4>",file=o)

    print("<pre>",file=o)
    printstd(completed.stdout,o)
    print("</pre>",file=o)

    print("<h4>STDERR</h4>",file=o)
    print("<pre>",file=o)
    printstd(completed.stderr,o)
    print("</pre>",file=o)
    print("<p>Python command",file=o)
    if completed.returncode == 0:
       okmessage("OK",o)
    else:
       errmessage("ERROR",o)
    print("</p>",file=o)

if args.others:
    tests=args.others

if args.c:
    print(Fore.RED + "DELETION OF ALL EXECUTABLES" + Style.RESET_ALL)
    ok = input("Are you sure ? ")
    if ok in ["ok","ok","yes","y"]:
        print(Fore.RED + "Deletion ..." + Style.RESET_ALL)
        for file in os.scandir("./build"):
            if file.is_file():
               withoutSuffix = re.sub(r'(_[0-9]+)$','',file.name)
               if withoutSuffix in tests:
                  print("Delete " + file.path)
                  os.remove(file.path)
    else:
        print(Fore.RED + "Cancelled !" + Style.RESET_ALL)

else:
    testsInDir={}
    nbtests=0
    # Scan all files and keep only the tests
    for file in os.scandir("./build"):
        if file.is_file():
           withoutSuffix = re.sub(r'(_[0-9]+)$','',file.name)
           # if it is a test which is not disabled we keep it
           if withoutSuffix in tests and not withoutSuffix in disabled:
              nbtests = nbtests + 1
              if withoutSuffix in testsInDir:
                 testsInDir[withoutSuffix].append(file.name)
              else:
                 testsInDir[withoutSuffix]=[file.name]
    
    with open(args.o,"w") as o:
        print(header,file=o)
        nb=0
        testSeen=False
        hasNotBuiltTest=False
    
        # Display not built tests
        # All tests which have not been seen in the directory
        for test in tests:
            if not (test in testsInDir):
                hasNotBuiltTest = True

        # If there are tests not built then we display
        # a section in the final report
        if hasNotBuiltTest:
           print("<h2>Tests not run</h2>",file=o)
           for test in tests:
               if not (test in testsInDir):
                  if test in disabled:
                    print("Test disabled : %s" % test,file=o)
                  else:
                    errmessage("Test not built : %s" % test,o)
    
        # Run built tests after the args.a one
        # or all if args.a not set

        # Scan all tests
        for test in tests:
            # If the test is args.a or args.a not defined then we start
            # taking into account the tests
            if args.a and args.a == test:
               testSeen = True
            # We run tests only AFTER (included) a given test in the list
            # If the test has been built
            if test in testsInDir:
               # We get the subtest and run them
               subTests = testsInDir[test]
               if not args.a or testSeen:
                   ptest("Test %s (%d / %d)" % (test,nb+1,nbtests),o)
                   for subtest in subTests:
                     if len(subTests)>1:
                        psubtest("Subtest %s " % subtest,o)
                     if not args.t:
                        runtest(subtest,o)
               # Increment test counter
               nb = nb + len(subTests)
        print(footer,file=o)
