#pragma once

#define SEX_MALE 1
#define SEX_FEMALE 2

#define LOAD_RESET 0
#define LOAD_INDI 1
#define LOAD_OTHER 2

#define MAX_SPOUSE 5
#define MAX_CHILDREN 20

#define READMODE_DEFAULT 0
#define READMODE_BIRTH 1
#define READMODE_DEATH 2

class CIndi
{
public:
	HTREEITEM hti;
	CString reference;
	CString name;
	CString givenName;
	CString surname;
	CString source;
	CString birthDate;
	CString birthPlace;
	CString deathDate;
	CString deathPlace;
	CIndi* father = NULL;
	CIndi* mother = NULL;;
	int childCnt = 0;
	CIndi* children[MAX_CHILDREN] = { NULL };
	COleDateTime Date;
	unsigned char sex = 0;
};

class CFam
{
public:
	CString reference;
	int husbCnt = 0;
	int wifeCnt = 0;
	int childCnt = 0;
	int dateCnt = 0;
	int placeCnt = 0;
	CString husbRef[MAX_SPOUSE];
	CIndi* husb[MAX_SPOUSE] = { NULL };
	CString wifeRef[MAX_SPOUSE];
	CIndi* wife[MAX_SPOUSE] = { NULL };
	CString childRef[MAX_CHILDREN];
	CIndi* child[MAX_CHILDREN] = { NULL };
	CString date[MAX_SPOUSE];
	CString place[MAX_SPOUSE];
};

class CGedData
{
private:
	CStdioFile file;
	CFileException exception;
	CString line;
public:
	CGedData();
	int load(wchar_t *filename);
	int startNewRecord(CString& line);
	int readIndiData(CString& identifier);
	int readFamData(CString& indentifier);
	int readUnknownSection(CString& line);
	int buildTree();
	CList<CIndi, CIndi&>indiList;
	CList<CFam, CFam&>famList;
};
