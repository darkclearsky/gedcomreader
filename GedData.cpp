#include "pch.h"
#include "GedData.h"

int CGedData::load(wchar_t *filename)
{
	unsigned char done = FALSE;

	if (file.Open(filename,CFile::modeRead,&exception)) {
		if (file.ReadString(line)) {
			while (!done) {
				switch (line[0])
				{
				case	'0':	// new record
					if (!startNewRecord(line))
						done = TRUE;
					break;
				default:
					continue;
				}
			}
		}
		file.Close();
		// CString FinalMsg;
		// FinalMsg.Format(_T("Total of %ld individuals loaded"), indiList.GetCount());
		// OutputDebugString(FinalMsg);
		return(TRUE);
	} else {
		return(FALSE);
	}
}

int CGedData::startNewRecord(CString& line)
{
	// TODO: Add your implementation code here.
	// split into three space separated paramters 
	CString token[2];
	int i = 1;
	int len = line.GetLength();
	token[0] = line.Tokenize(_T(" "), i);
	if (i != -1) { // get rest of record 
		token[1] = line.Right(len - i);
		token[1].TrimRight();
	}

	if (token[0].Compare(_T("TRLR")) == 0) {
		return(FALSE);	// done
	} else {
		if (token[1].Compare(_T("INDI")) == 0) {
			return(readIndiData(token[0]));
		} else {
			if (token[1].Compare(_T("FAM")) == 0) {
				return(readFamData(token[0]));
			} else {
				return(readUnknownSection(token[0]));
			}
		}
	}
}

int CGedData::readIndiData(CString& identifier)
{
	CIndi indi;
	CString token[2];
	int i;
	int len;
	char ReadMode = READMODE_DEFAULT;

	indi.reference = identifier;
	while (file.ReadString(line)) {
		if (line[0] == '0') { // end of indi record
			indiList.AddTail(indi);
			return(TRUE);
		}
		if (line[0] == '1') {
			ReadMode = READMODE_DEFAULT;
		}
		len = line.GetLength();
		i = 1;
		token[0] = line.Tokenize(_T(" "), i);
		if (i != -1) { // get rest of record
			token[1] = line.Right(len - i);
			token[1].TrimRight();

			if (ReadMode == READMODE_BIRTH) {
				if (token[0].Compare(_T("DATE")) == 0) {
					indi.birthDate = token[1];
				} else {
					if (token[0].Compare(_T("PLAC")) == 0) {
						indi.birthPlace = token[1];
					}
				}
				continue;
			}
			if (ReadMode == READMODE_DEATH) {
				if (token[0].Compare(_T("DATE")) == 0) {
					indi.deathDate = token[1];
				} else {
					if (token[0].Compare(_T("PLAC")) == 0) {
						indi.deathPlace = token[1];
					}
				}
				continue;
			}

			if (token[0].Compare(_T("BIRT")) == 0) {
				ReadMode = READMODE_BIRTH;
				continue;
			}
			if (token[0].Compare(_T("DEAT")) == 0) {
				ReadMode = READMODE_DEATH;
				continue;
			}
			if (token[0].Compare(_T("NAME")) == 0) {
				indi.name = token[1];
			} else {
				if (token[0].Compare(_T("GIVN")) == 0) {
					indi.givenName = token[1];
				} else {
					if (token[0].Compare(_T("SURN")) == 0) {
						indi.surname = token[1];
					} else {
						if (token[0].Compare(_T("SOUR")) == 0) {
							indi.source = token[1];
						} else {
							if (token[0].Compare(_T("SEX")) == 0) {
								if (token[1].Compare(_T("M")) == 0) {
									indi.sex = SEX_MALE;
								} else {
									indi.sex = SEX_FEMALE;
								}
							}
						}
					}
				}
			}
		}
	}
	indiList.AddTail(indi);
	return(FALSE);
}

int CGedData::readFamData(CString& identifier)
{
	CFam fam;
	CString token[2];
	int i;
	int len;
	fam.reference = identifier;
	while (file.ReadString(line)) {
		if (line[0] == '0') { // end of indi record
			famList.AddTail(fam);
			return(TRUE);
		}
		len = line.GetLength();
		i = 1;
		token[0] = line.Tokenize(_T(" "), i);
		if (i != -1) { // get rest of record
			token[1] = line.Right(len - i);
			token[1].TrimRight();
			if (token[0].Compare(_T("HUSB")) == 0) {
				// fam.husbRef.Add(token[1]);
				if(fam.husbCnt < MAX_SPOUSE)
					fam.husbRef[fam.husbCnt++] = token[1];
			} else {
				if (token[0].Compare(_T("WIFE")) == 0) {
					// fam.wifeRef.Add(token[1]);
					if(fam.wifeCnt < MAX_SPOUSE)
						fam.wifeRef[fam.wifeCnt++] = token[1];
				} else {
					if (token[0].Compare(_T("CHIL")) == 0) {
						// fam.childRef.Add(token[1]);
						if(fam.childCnt < MAX_CHILDREN)
							fam.childRef[fam.childCnt++] = token[1];
					} else {
						if (token[0].Compare(_T("DATE")) == 0) {
							// fam.date.Add(token[1]);
							if(fam.dateCnt < MAX_SPOUSE)
								fam.date[fam.dateCnt++] = token[1];
						} else {
							if (token[0].Compare(_T("PLAC")) == 0) {
								// fam.place.Add(token[1]);
								if(fam.placeCnt < MAX_SPOUSE)
									fam.place[fam.placeCnt++] = token[1];
							}
						}
					}
				}
			}
		}
	}
	famList.AddTail(fam);
	return(FALSE);
}

int CGedData::readUnknownSection(CString& identifier)
{
	while (file.ReadString(line)) {
		if (line[0] == '0') {
			return(TRUE);
		}
	}
	return(FALSE);
}

int CGedData::buildTree()
{
	int x, y, z;
	// CFam& fam = CFam();
	// First pass - go through family records - based on the tags, go through individual records and match up links
	POSITION fampos = famList.GetHeadPosition();
	while (fampos != NULL) {
		CFam& fam = famList.GetNext(fampos);
		// OutputDebugString(_T("Family ") + fam.reference + _T("\n"));
		POSITION indipos = indiList.GetHeadPosition();
		// CIndi& indi;
		while (indipos != NULL) {
			// indi = indiList.GetNext(indipos);
			CIndi& indi = indiList.GetNext(indipos);
#if 0
			// **_** debug
			if ((fam.reference.Compare(_T("@F43@")) == 0) && (indi.surname.Compare(_T("Hansen")) == 0)) {
				OutputDebugString(_T("Found fam @F43@ and indi Hansen\n"));
			}
			// end debug
#endif
			for (x = 0; x < fam.husbCnt; x++) {
				if (fam.husbRef[x].Compare(indi.reference) == 0) {	// found a husband
					fam.husb[x] = &indi; // save it off
					// OutputDebugString(_T("Found a husband ") + indi.name + _T("\n"));
				}
			}
			for (x = 0; x < fam.wifeCnt; x++) {
				if (fam.wifeRef[x].Compare(indi.reference) == 0) { // found a wife
					fam.wife[x] = &indi; // save it off
					// OutputDebugString(_T("Found a wife ") + indi.name + _T("\n"));
				}
			}
			for (x = 0; x < fam.childCnt; x++) {
				if (fam.childRef[x].Compare(indi.reference) == 0) { // found a child
					fam.child[x] = &indi; // save it off
					// OutputDebugString(_T("Found a child ") + indi.name + _T("\n"));
				}
			}
		}
	}
	// now go through famList again and use indi pointers to create final family tree...
	fampos = famList.GetHeadPosition();
	while (fampos != NULL) {
		// fam = famList.GetNext(fampos);
		CFam& fam = famList.GetNext(fampos);
		// find fathers and mothers for the children, 
		// for now always use first father and mother (won't always be true in some cases)
		if (fam.husbCnt > 0) {	
			for (x = 0; x < fam.childCnt; x++) {
				// assign father from husband of family
				if (fam.child[x] != NULL) {
					fam.child[x]->father = fam.husb[0];
					// OutputDebugString(_T("Assigning ") + fam.child[x]->name + _T(" a father ") + fam.husb[0]->name + _T("\n"));
				} else {
					OutputDebugString(_T("Warning - NULL child\n"));
				}
				// assign children to their father (husband of family)
				if (fam.husb[0] != NULL) {
					fam.husb[0]->children[x] = fam.child[x];
					fam.husb[0]->childCnt++;
					/*
					if (fam.child[x] != NULL) {
						OutputDebugString(_T("Assigning ") + fam.husb[0]->name + _T(" a child ") + fam.child[x]->name + _T("\n"));
					}
					*/
				}
			}
		}
		if (fam.wifeCnt > 0) {
			for (x = 0; x < fam.childCnt; x++) {
				// assign mother from wife of family
				if (fam.child[x] != NULL) {
					fam.child[x]->mother = fam.wife[0];
				}
				// assign children to their mother (wife of family)
				if (fam.wife[0] != NULL) {
					fam.wife[0]->children[x] = fam.child[x];
					fam.wife[0]->childCnt++;
				}
			}
		}
	}
	return(TRUE);
}

CGedData::CGedData()
{
	return;
}