/*
    Compress-tool main class.
    This provides a CLI to manipulate coding/compression algorithms

    Copyright (C) 2010  Ludovic Favre <ludovic.favre@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <cstdlib>
#include <iostream>
#include <string>
#include <csignal>
#include "codecs/BWT.hpp"
#include "codeschemes/Lz.hpp"
#include "codeschemes/LzNoReset.hpp"
#include "codeschemes/HuffBwt.hpp"
#include "codeschemes/HuffMtfBwt.hpp"
#include "codeschemes/HuffRleBwt.hpp"
#include "codeschemes/HuffMtfRleBwt.hpp"
#include "codeschemes/HuffRleMtfBwt.hpp"
#include "defines.hpp"
#include <getopt.h> //required for option parsing
#define VERSION "1.4"
//Usage : see -h option

#undef VERBOSE
bool decompress = false; // default = compress

using namespace std;
#define SET    0
#define LZ78_ALG 17
#define LZ78NR_ALG 18
#define BW_ALG   19
#define BH_ALG       100
#define BMH_ALG	 101
#define BRH_ALG      102
#define BMRH_ALG     103
#define BRMH_ALG     104
#define DEFAULT SET
int format = DEFAULT;

static inline void debug(std::string text, int level);
int parseCmd(int argc, char** argv);
void displayHelp(std::string);
void sigfunc(int sig);
void cleanfunc(int sig);
static ICodec* compressor = NULL;
static string* output = NULL;


struct globalArgs_t {
    unsigned int help; /* -h option*/
    bool decompress;	/* -d option*/
    unsigned int algorithm; /* -t option */
    bool toStdout ; /*-stdout option*/
    bool fromStdin ; /*-stdin option*/
    string* infile;
    int verbosity;              /* -v option */
} globalArgs;

static const char *optString = "vhdt:;";//"Il:o:vh?";

static const struct option longOpts[] = {
    { "help", no_argument, NULL, 'h' },
    { "decompress", no_argument, NULL, 'd' },
    { "algorithm", required_argument, NULL, 't' },
    { "stdin", no_argument, NULL, 0 },
    { "stdout", no_argument, NULL, 0 },
    { "verbose", no_argument, NULL, 'v' },
    {NULL, 0, NULL, 0}
};

/*
 *
 */
int main(int argc, char** argv) {

    (void) signal(SIGINT, sigfunc);
    (void) signal(SIGSEGV, cleanfunc);
    (void) signal(SIGTERM, cleanfunc);
    (void) signal(SIGKILL, cleanfunc);

    // stdin/stdout is no longer supported since version 1.0. This breaks Boost Unit Tests
    // can be manually reactivated but hasn't been tested.
    globalArgs.fromStdin = false;
    globalArgs.toStdout = false;
    if (parseCmd(argc,argv)==-1) {
        debug("Parsign error, terminating...",CRITICAL);
        goto Terminate;
    };

    if (!globalArgs.algorithm) {
        debug("No format chosen, defaulting to LZ78",INFO);
        globalArgs.algorithm = LZ78_ALG;
    }

    switch (globalArgs.algorithm) {
    case BH_ALG : {
        debug("Format chosen: BH_ALG", DEBUG);
        if (globalArgs.fromStdin) {
            compressor = new HuffBwt(NULL, debug, globalArgs.verbosity,globalArgs.decompress);
        } else {
            if (globalArgs.infile == NULL) {
                debug("You must provide a file to (de)compress. For more informations, use -h option", INFO);
                return EXIT_FAILURE;
            } else {
                compressor = new HuffBwt(globalArgs.infile, debug, globalArgs.verbosity,globalArgs.decompress);
            }
        }
        break;
    }

    case BMH_ALG : {
        debug("Format chosen: BMH_ALG", DEBUG);
        if (globalArgs.fromStdin) {
            compressor = new HuffMtfBwt(NULL, debug, globalArgs.verbosity,globalArgs.decompress);
        } else {
            if (globalArgs.infile == NULL) {
                debug("You must provide a file to (de)compress. For more informations, use -h option", INFO);
                return EXIT_FAILURE;
            } else {
                compressor = new HuffMtfBwt(globalArgs.infile, debug, globalArgs.verbosity,globalArgs.decompress);
            }
        }
        break;
    }

    case BRH_ALG : {
        debug("Format chosen: BRH_ALG", DEBUG);
        if (globalArgs.fromStdin) {
            compressor = new HuffRleBwt(NULL, debug, globalArgs.verbosity,globalArgs.decompress);
        } else {
            if (globalArgs.infile == NULL) {
                debug("You must provide a file to (de)compress. For mor informations, use -h option", INFO);
                return EXIT_FAILURE;
            } else {
                compressor = new HuffRleBwt(globalArgs.infile, debug, globalArgs.verbosity,globalArgs.decompress);
            }
        }
        break;
    }

    case BRMH_ALG : {
        debug("Format chosen: BRMH_ALG", DEBUG);
        if (globalArgs.fromStdin) {

            compressor = new HuffMtfRleBwt(NULL, debug, globalArgs.verbosity,globalArgs.decompress);
        } else {
            if (globalArgs.infile == NULL) {
                debug("You must provide a file to (de)compress. For more informations, use -h option", INFO);
                return EXIT_FAILURE;
            } else {
                compressor = new HuffMtfRleBwt(globalArgs.infile, debug, globalArgs.verbosity,globalArgs.decompress);
            }
        }
        break;
    }

    case BMRH_ALG : {
        debug("Format chosen: BRMH_ALG", DEBUG);
        if (globalArgs.fromStdin) {

            compressor = new HuffRleMtfBwt(NULL, debug, globalArgs.verbosity,globalArgs.decompress);
        } else {
            if (globalArgs.infile == NULL) {
                debug("You must provide a file to (de)compress. For more informations, use -h option", INFO);
                return EXIT_FAILURE;
            } else {
                compressor = new HuffRleMtfBwt(globalArgs.infile, debug, globalArgs.verbosity,globalArgs.decompress);
            }
        }
        break;
    }


    case LZ78_ALG : {
        debug("Format chosen: LZ78_ALG", DEBUG);
        if (globalArgs.fromStdin) {
            compressor = new Lz(NULL, debug, globalArgs.verbosity,globalArgs.decompress);
        } else {
            if (globalArgs.infile == NULL) {
                debug("You must provide a file to (de)compress. For more informations, use -h option", INFO);
                return EXIT_FAILURE;
            } else {
                compressor = new Lz(globalArgs.infile, debug, globalArgs.verbosity,globalArgs.decompress);
            }
        }
        break;
    }

    case LZ78NR_ALG : {
        debug("Format chosen: LZ78NR_ALG", DEBUG);
        if (globalArgs.fromStdin) {
            compressor = new LzNoReset(NULL, debug, globalArgs.verbosity,globalArgs.decompress);
        } else {
            if (globalArgs.infile == NULL) {
                debug("You must provide a file to (de)compress. For more informations, use -h option", INFO);
                return EXIT_FAILURE;
            } else {
                compressor = new LzNoReset(globalArgs.infile, debug, globalArgs.verbosity,globalArgs.decompress);
            }
        }
        break;
    }
    default:
        break;
    }
    if (compressor !=NULL) {
        debug("Codec is ready !", INFO);
        if (globalArgs.decompress) {
            debug("Decompressing ! ", DEBUG);
            if (globalArgs.toStdout || globalArgs.infile == NULL) {
                compressor->decompress(NULL);
            } else {
                compressor->decompress(globalArgs.infile);
                delete output;
                output = NULL;
            }
        } else {
            debug("Compressing ", DEBUG);
            if (globalArgs.toStdout || globalArgs.infile == NULL) {
                compressor->compress(NULL);
            } else {
                output = new string(*globalArgs.infile);
                output->append(".cprs");
                compressor->compress(output);
                delete output;
                output = NULL;
            }

        }
        delete compressor;
    }

Terminate:
    if (output !=NULL) {
        delete output;
        output=NULL;
    }
    delete globalArgs.infile;
    return (EXIT_SUCCESS);
}

static inline void debug(std::string text, int level = INFO) {
    if (globalArgs.verbosity >= level) {
        fprintf(stderr, "%s\n", text.c_str());
    }
}
int parseCmd(int argc, char** argv)

{

    int longIndex;
    bool filtered = false;
    int opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    globalArgs.algorithm = 0;
    while ( opt != -1 ) {
        switch ( opt ) {
        case 't':
            //fprintf(stderr,"-t");
            if ( strcmp( "lz78", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = lz78\n");
                globalArgs.algorithm = LZ78_ALG;
            }
            if ( strcmp( "lz78nr", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = lz78NR\n");
                globalArgs.algorithm = LZ78NR_ALG;
            }
            if ( strcmp( "bw", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = bw\n");
                globalArgs.algorithm = BW_ALG;
            }
            if ( strcmp( "bh", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = bh\n");
                globalArgs.algorithm = BH_ALG;
            }
            if ( strcmp( "bmh", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = bmh\n");
                globalArgs.algorithm = BMH_ALG;
            }
            if ( strcmp( "bmrh", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = bmrh\n");
                globalArgs.algorithm = BMRH_ALG;
            }
            if ( strcmp( "brmh", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = bmrh\n");
                globalArgs.algorithm = BRMH_ALG;
            }
            if ( strcmp( "brh", optarg ) == 0 ) {
                //fprintf(stderr,"Alg = brh\n");
                globalArgs.algorithm = BRH_ALG;
            }
            if (globalArgs.algorithm == 0) {
                debug("Invalid algorithm option, defaulting to lz78\n",INFO);
                globalArgs.algorithm = LZ78_ALG;
            }
            break;

        case 'v':
            globalArgs.verbosity++;
            fprintf(stderr,"Verbosity = %d\n",globalArgs.verbosity);
            break;

        case 'd':
            globalArgs.decompress = true;
            fprintf(stderr,"Decompress..");
            break;

        case 'h':   /* fall-through is intentional */
        case '?':
            displayHelp(std::string(argv[0]));
            break;
        case 0:     /* long option without a short arg */
            //fprintf(stderr,"case 0");
            filtered = false;
            if ( strcmp( "stdin", longOpts[longIndex].name ) == 0 ) {
                globalArgs.fromStdin = true;
                filtered = true;
                fprintf(stderr,"******************STDOUT");
            }

            if ( strcmp( "stdout", longOpts[longIndex].name ) == 0 ) {
                globalArgs.toStdout = true;
                filtered = true;
                fprintf(stderr,"******************STDOUT");
            }
            if (!filtered) {

                globalArgs.infile = new std::string(longOpts[longIndex].name);
                fprintf(stderr,"filename = %s\n",globalArgs.infile->c_str());
            }
            break;

        default:
            /* You won't actually get here. */
            fprintf(stderr,"Error parsing arguments...");
            return -1;
            break;
        }

        opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    }
    for ( ; optind < argc; optind++) {
        if (globalArgs.infile == NULL) {
            if (access(argv[optind], F_OK)==0) {
                if (access(argv[optind], R_OK)==0) {

                    globalArgs.infile = new std::string(argv[optind]);

                }
                else {
                    debug("Cannot read "+std::string(argv[optind]),CRITICAL);
                    return -1;
                }
            }
            else {
                debug("Cannot find file : "+std::string(argv[optind]),CRITICAL);
                return -1;
            }


        }
        else {
            debug("Input file already specified ! ",CRITICAL);
        }
    }
    return 0;
}
void displayHelp(std::string progname) {
    std::cout << progname <<std::endl;
    std::cout <<"\nDescription: \tCompression utility that provides LZ78 and Burrows-Wheeler based algorithms for benchmarking"<<std::endl;
    std::cout << "\nVersion : " << VERSION << std::endl;
    std::cout <<"License: GPLv3"<< std::endl;
    std::cout <<"Copyright (C) 2010  \n\nAuthor : Ludovic Favre <ludovic.favre@epfl.ch>" << std::endl;
    std::cout <<"\nThanks to:\n\t Ghid Maatouk : Project Supervisor"<<
              "\n\t Masoud Alipour : Implementation suggestions"<<
              "\n\t Prof. Amin Shokrollahi : Head of ALGO and LMA "<<std::endl;


    std::cout << "\nUsage : \n "<<progname <<" <options> filename" << std::endl;
    std::cout << "\nOptions : "<<std::endl;
    std::cout << "\t-h \t\tDisplay this help text" << std::endl;
    std::cout << "\t-d \t\tDecompress" << std::endl;
    std::cout << "\t-t <scheme>\tCompression format (see below for more)" << std::endl;
    std::cout << "\t-v \t\tIncrease verbosity (default is 0, max is 3)" << std::endl;
    //std::cout << "\t-quiet \t\tDo not print anything in the console" << std::endl;
    //std::cout << "\t--stdout \tOutput to stdout" << std::endl; //see remark at beginning of code
    //std::cout << "\t--stdin \tInput from stdin" << std::endl;
    std::cout << "\n\tCompression schemes :"<<std::endl;
    std::cout << "\t\t<scheme>\t  Description"<<std::endl << std::endl;
    std::cout << "\t\tlz78\t\t: LZ78(Input)"<<std::endl;
    std::cout << "\t\tbmh\t\t: Huffman((MTF(BWT(Input)))"<<std::endl;
    std::cout << "\t\tbrh\t\t: Huffman((RLE(BWT(Input)))"<<std::endl;
    std::cout << "\t\tbmrh\t\t: Huffman(RLE(MTF(BWT(Input)))) "<<std::endl;
    std::cout << "\t\tbrmh\t\t: Huffman(MTF(RLE(BWT(Input))))"<<std::endl;
    std::cout << "\n\n\tOther(s) scheme(s) :"<<std::endl;
    std::cout << "\t\tbh\t\t: Huffman(BWT(Input)) No designed to compress or even work, test purpose only !"<<std::endl;
    std::cout << "\t\tlz78nr\t\t: LZ78NoDictReset(Input) LZ78 without dictionary reset. Shown as less efficient in report"<<std::endl;
}

void sigfunc(int sig) {
    if (globalArgs.infile != NULL) {
        delete globalArgs.infile;
        globalArgs.infile = NULL;
    }

    if (output != NULL) {
        delete output;
        output = NULL;
    }

    if (compressor != NULL) {
        delete compressor;
        compressor = NULL;
    }
    fprintf(stderr, "Signal %d\nYou have presses Ctrl-C , please press again to exit",sig);
    (void) signal(SIGINT, SIG_DFL);
}

void cleanfunc(int sig) {
    if (globalArgs.infile != NULL) {
        delete globalArgs.infile;
        globalArgs.infile = NULL;
    }

    if (output != NULL) {
        delete output;
        output = NULL;
    }

    if (compressor != NULL) {
        delete compressor;
        compressor = NULL;
    }
    if (sig == SIGSEGV) {
        fprintf(stderr, "An error occured (SEGFAULT), program terminated\n");
    }

    if (sig != SIGKILL) {
        kill(getpid(), SIGKILL);
    }
}


