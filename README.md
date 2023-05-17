# gedcomreader
GEDCOM file reader

GEDCOM is a file format standard for geneology data interchange.

This is the beginnings of a gedcom file reader for Windows.  Very far from finished or even very functional.  So far it reads the file, interprets some of the data (partial portions of the INDI records and FAM records) - and builds a relational tree from that data but only the direct lineage from down of the first gedcom record.

I've only ever run this with my own gedcom data and much of this code makes shortcuts and it only processes a small amount of the actual data that is in a gedcom file.  Very much work in progress.
