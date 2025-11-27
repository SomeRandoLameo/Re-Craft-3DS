import os
import sys

DEVKITPRO = os.environ.get("DEVKITPRO")
if not DEVKITPRO:
    sys.exit("Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitpro")
    
import subprocess
import multiprocessing
from colorama import Fore, Style
import shutil
import argparse

def getBuildPath():
	return "build/"

def main() -> None:
    def status(msg: str):
        print(Fore.CYAN + msg + Fore.RESET + Style.RESET_ALL)

    parser = argparse.ArgumentParser('build.py', description="Build the Re-Craft-3DS project")
    parser.add_argument('-c', action='store_true', help="Clean before building")
    parser.add_argument('-v', action='store_true', help="Give verbose output")
    parser.add_argument('-r', action='store_true', help="Optimize for release build (Default: Debug)")
    parser.add_argument('-t', action='store_true', help="Specify custom target")
    parser.add_argument('-s', action='store_true', help="Send file after building (overrides -t)")
    parser.add_argument('-w', action='store_true', help="Do not hide warnings (Ok)")
    parser.add_argument('-a', action='store_true', help="Show all warnings ( Abfahrt. )")
    parser.add_argument('-u', action='store_true', help="Show every warning possible for gcc (  Eskalationsstufe 9: Gemeinsamer Untergang.  )")
    args = parser.parse_args()

    # Configure
    if args.c:
        shutil.rmtree(getBuildPath(), ignore_errors=True)
    cache_file = os.path.join(getBuildPath(), "CMakeCache.txt")
    if os.path.exists(cache_file):
        os.remove(cache_file)

    cmake_args = [
        'cmake',
        '-S', '.',
        '-B', getBuildPath(),
        '-G', 'Unix Makefiles',
        f"-DCMAKE_TOOLCHAIN_FILE={DEVKITPRO}/cmake/3DS.cmake"
    ]
    if args.r:
        cmake_args.append("-DCMAKE_BUILD_TYPE=Release")
    else:
        cmake_args.append("-DCMAKE_BUILD_TYPE=Debug")

    if args.u:
        cmake_args.append("-DEXTRAWARNINGS=1")
    if args.a:
        cmake_args.append("-DALLWARNINGS=1")
    if args.w:
        cmake_args.append("-DWARNINGS=1")

    try:
        subprocess.run(cmake_args, check=True)
    except subprocess.CalledProcessError:
        exit(1)

    # Build
    make_args = ['cmake', '--build', getBuildPath()]

    if args.s:
        make_args.append("-tsend")
    if args.t and not args.s:
        make_args.append("-t"+args.t)
    if args.v:
        make_args.append("--verbose")

    try:
        subprocess.run(make_args, check=True)
    except subprocess.CalledProcessError:
        exit(1)

if __name__ == "__main__":
    main()
