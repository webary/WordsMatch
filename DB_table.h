#pragma once
#ifndef __DB_TABLE_H__
#define __DB_TABLE_H__

struct TABLE
{
	CString tableName;
	CString tvalueName[20];
};

class DB_table : public CDialog
{
	DECLARE_DYNAMIC(DB_table)
public:
	const CString m_dbName;
	const _bstr_t DBName;
	static const int tableNum = 1;
	TABLE table[tableNum];

	DB_table(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DB_table();

	CString findData(const char* s1, int re_i = 1);
	void addRow(const char* s1, CString s2 = "", CString s3 = "");

// 对话框数据
	//enum { IDD = IDD_DB1_FORM };
	static const int TValueNums = 3;	//当前表的列数
	char source[31];

private:
	int dataLen[tableNum];	//当前每个表中数据个数
	const CString LogDir;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif // !__DB_TABLE_H__