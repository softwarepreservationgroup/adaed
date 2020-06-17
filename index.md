---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults

layout: home
---
# NYU Ada/Ed project

Nigel Williams and Paul McJones (<paul@mcjones.org>), editors.

<blockquote>
<p>Ada/Ed is a translator-interpreter for Ada 83. It is
intended as a teaching tool, and does not have the capacity,
performance, or robustness of commercial Ada compilers. Ada/Ed was
developed at New York University, as part of a long-range project in
language definition and software prototyping. The project produced the
first validated translator for Ada, in the form of an executable
definition of the language written in SETL. The SETL system served as
design document and prototype for the C version
[available from the NYU host].</p>

<p>Ada/Ed was last validated under version 1.7 of the ACVC tests (Ada
83). Therefore it is not currently a validated Ada system, and users
can expect to find discrepancies between Ada/Ed and currently
validated compilers (and none of the new Ada 95 mechanisms are
implemented, of course).</p>

<p>Apart from the  100-odd tests of ACVC 1.11 that  Ada/Ed failed, the
major deficiency of the system is  that, being an interpreter, it does
not implement most  representation clauses, and thus  does not support
systems programming close to the machine level.</p>

<p>[From README ...]</p>
</blockquote>


## Original NYU Ada/Ed Compiler, written in SETL

### Version 19.7, of March 21, 1983

* SofTech, Inc, Ada Compiler Validation Summary Report: NYU Ada/ED,
Version 19.7.  V-001, April 11, 1983.
[PDF at dtic.mil](https://apps.dtic.mil/dtic/tr/fulltext/u2/a136759.pdf)
<blockquote><p>The New York University Ada translator (NYU Ada/Ed)
version 19.7 (March 21, 1983), was tested with version 1.1 (March 4,
1983) of the ACVC validation tests.
1.1 of the test suite contained 1,633 tests, of which 1,325 were applicable to
Version NYU Ada/Ed. Of the applicable tests, 14 were withdrawn, due to errors in the
tests. NYU Ada/Ed passed all of the remaining 1,311 applicable correct tests.</p></blockquote>


### Version 1.4, validated June 28th, 1984

The source code of this version was published as a two-part NYU technical report:

* Ada Project. ADASEM: Static Semantics for Ada. Listing of the NYU
  Ada/Ed compiler, Version 1.4, validated June 28, 1984. Courant
  Institute, New York University, July 3, 1984. [PDF](setl/ADASEM_STL-1.4.pdf)

* Ada Project. Ada/Ed INTERPRETER: Executable semantic model for
  Ada. Listing of the NYU Ada/Ed compiler, Version 1.4, validated June
  28, 1984. Courant Institute, New York, July 3, 1984.
  University. [PDF](setl/ADAI_STL-1.4.pdf)

The project was described in these papers:

* Philippe Kruchten and Edmond Schonberg. The ADA/ED System: A
  Large-Scale Experiment in Software Prototyping Using SETL. Pages
  398-515 in: Budde R., Kuhlenkamp K., Mathiassen L., Züllighoven
  H. (eds) Approaches to Prototyping. Springer, Berlin,
  Heidelberg, 1984. [SpringerLink](https://doi.org/10.1007/978-3-642-69796-8_34)

* Heinrich P. Godbersen. Comments on "The Ada/Ed System: A Large-Scale
  Experiment in Software Prototyping Using SETL" by P. Kruchten and
  E. Schonberg. Pages 416-417 in Budde R., Kuhlenkamp K., Mathiassen
  L., Züllighoven H. (eds) Approaches to Prototyping. Springer,
  Berlin,
  Heidelberg, 1984. [SpringerLink](https://doi.org/10.1007/978-3-642-69796-8_35)

* Peter Schnupp. Comments on "The ADA/ED System: A Large-Scale
  Experiment in Software Prototyping Using SETL" by P. Kruchten and
  E. Schonberg. Pages 418-423 in Budde R., Kuhlenkamp K., Mathiassen
  L., Züllighoven H. (eds) Approaches to Prototyping. Springer,
  Berlin,
  Heidelberg, 1984. [SpringerLink](https://doi.org/10.1007/978-3-642-69796-8_36)

* Philippe Kruchten, Edmond Schonberg, and Jacob Schwartz. Software
  prototyping using the SETL programming language. _IEEE Software_,
  1(4):66–75, October 1984. [IEEE Xplore](https://doi.org/10.1109/MS.1984.229465)


### Version 1.10, 1989

This version was validated on a Sun-3/60 in 1989:

* Wright-Patterson AFB. Ada Compiler Validation Summary Report: New
  York NYU Ada/Ed, Version 1.10, Sun-3/60 (Host & Target),
  890523W1.10085. May 22, 1989. [Online at apps.dtic.mil](https://apps.dtic.mil/sti/citations/ADA211934)

Version 1.10 was distributed through the National Technical
Information Service for VAX computers running VMS Version 5.2 and UNIX
(version unknown). Here are reports describing the availability of
distribution tapes:

* Stephen P. Wilson. Ada/Ed Compiler, Version 1.10(VAX). Aeronautical
  Systems Division, Wright-Patterson AFB, Ohio, January 29, 1990.
  [Online at dtic.mil](https://apps.dtic.mil/sti/citations/ADA218687)

* Stephen P. Wilson. Ada/Ed Compiler, Version 1.10(UNIX). Aeronautical
  Systems Division, Wright-Patterson AFB, Ohio, January 29, 1990.
  [Online at dtic.mil](https://apps.dtic.mil/sti/citations/ADA218688)


## NYU Ada/Ed-C

Ada/Ed-C was a rewrite from SETL to C, for Unix-based machines. This
paper describes the design process:

* E. Schonberg and D. Shields. From prototype to efficient
  implementation: A case study using SETL and C. Technical Report 170,
  Courant Institute of Mathematical Sciences, New York University,
  July 1985. [PDF](c/Schonberg_Shields-From_Prototype_to_Efficent_Implementation-1985.pdf)
  / [archive.org](https://archive.org/details/fromprototypetoe00scho)


### NYUADA project. Ada/Ed-C version 1.11.0a, 1992

Version 11.11.0a could be built for various Unix systems; see #ifdefs
in config.h.

* NYU Ada project, version 11.11.0a source. February 7, 1992.
  [files](c/adaed.1.11.0a) /
  [.tar.gz and README online at ftp.nice.ch](https://ftp.nice.ch/pub/next/developer/languages/ada)
  /
  [mirror](RAW/ftp.nice.ch/pub/next/developer/languages/ada/Adaed.1.11.s.tar.gz)

* Ragnar Hojland. Full sources, with "a couple of tweaks" for
  Linux 2.2 glibc2. Hojland changed the version number to 1.12.
  [files](c/adaed.1.11.0a-Hojland/) /
  [.tar.gz at ibiblio.org](http://www.ibiblio.org/pub/Linux/devel/lang/ada/Adaed-1.12.tar.gz) /
  [mirror](RAW/www.ibiblio.org/pub/Linux/devel/lang/ada/Adaed-1.12.tar.gz)

* Rolf Ebert. Linux executables and libraries, and a file of diffs to adapt the
  sources to Linux, circa 1992. Ebert is acknowledged in the
  release.notes file of version 1.11.2 as having contributed patches.
  [files](c/adaed.1.11.0a-Ebert/) /
  [adaed.README and adaed.tar.z at ibiblio.org](http://www.ibiblio.org/pub/Linux/devel/lang/ada/)
  / [mirror](www.ibiblio.org/pub/Linux/devel/lang/ada/)

* Dylan Kucera. Amiga executables and libraries, with only modified
  source files, circa 1993. Kucera changed the version number
  to 1.11.0b.  [files](c/adaed.1.11.0a-Kucera] /
  [Online at aminet.net](http://aminet.net/dev/lang/adaed.1.11.0b.lha)
  / [mirror](RAW/aminet.net/dev/lang/adaed.1.11.0b.lha)

* Eric Baigar. SGI Irix port. MIPS executables in SGI tardist format.
  [Online at baigar.de](http://www.baigar.de/irix/AdaEd-1.12.tardist)
  / [mirror](RAW/www.baigar.de/irix/AdaEd-1.12.tardist)


### NYUADA project. Ada/Ed-C system version 1.11.2, September 30, 1992

Version 1.11.2, as released by the NYUADA project, could be built for
the following targets:

- Silicon Graphics IRIS running IRIX
- Sun3 running SunOS
- Sun Sparcstation running SunOS
- RS6000/AIX
- Decstation running ULTRIX
- VAX running BSD 4.3 UNIX
- 386/DOS machines (with at least 2 megabytes of RAM)

In addition, the GW-Ada/Ed project at The George Washington
University, led by Professor Michael B. Feldman, produced the
GW-Ada/Ed Program Development Environment for Macintosh and MS-DOS
that extenderd the Ada/Ed interpreter with an editor and a "shell" to
control compiling, binding and execution of an Ada program.

* NYUADA and GWUADA projects. Ada/Ed 1.11.2 source code and
  executables for PC and Macintosh. 1992-1995.
  - All files. The nyu directory contains two versions of the original
    files, apparently only differing by some renaming to produce
    shorter filenames. The gwu directory contains subdirectories for
    DOS and Macintosh; within the Macintosh subdirectory there are
    several copies of most source files for unknown
    reasons. [files](c/adaed.1.11.2)
  - The Public Ada Library, Walnut Creek CD-ROM, November 1997.
  [Two ISO images at archive.org](https://archive.org/details/ADA_-_The_Public_Ada_Library_Walnut_Creek_November_1997)
  - [mirror](RAW/Public_Ada_Library_1997_11/ada/compiler/adaed)

* Michael B. Feldman, Charles W. Kann, Arthur Vargas Lopes, and Manuel
  A. Pérez-Quiñones. GW-Ada/Ed: free Ada 83 development
  environments for IBM PC-compatible and Apple Macintosh computers. In
  _Proceedings of the conference on TRI-Ada ’95: Ada’s role in global
  markets: solutions for a changing complex world (TRI-Ada
  ’95)_. Association for Computing Machinery, New York, NY, USA,
  446–454. [ACM Digital Library](https://doi.org/10.1145/376503.376719)


## NYU Ada/Ed-SETL2

In the late 1980s, W. Kirk Snyder designed and implemented a successor
to SETL called SETL2. A version of the original Ada/Ed was modified
for SETL2. [Mentioned in Bacon 2000, page 21.]

* Ada project. Ada/Ed source code. SETL2, circa 1991.
  [files](setl2/AdaEdSETL2/) /
  [.bz2 at setl.org](http://setl.org/AdaEdSETL2-orig.tar.bz2) /
  [mirror](RAW/setl.org/AdaEdSETL2-orig.tar.bz2)

* David Bacon. SETL for Internet Data Processing. Ph.D. thesis,
  Computer Science Department, New York University, January, 2000.
  [PDF at cs.nyu.edu](https://cs.nyu.edu/media/publications/bacon_david.pdf)
  
