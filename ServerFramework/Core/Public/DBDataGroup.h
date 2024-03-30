
BEGIN(Core)

USING_SHARED_PTR(USqlColumn);
USING_SHARED_PTR(UIndex);

//USING_SHARED_PTR(Table);
//USING_SHARED_PTR(Procedure);

/*
@ Date: 2024-03-25, Writer: 박태현
@ Explain: Sql을 도와주는 함수 모음
*/
class USqlHelpers
{
public:
	static _wstring		Format(const _tchar* _format, ...);
	static _wstring		DataType2String(SQLDATATYPE _type);
	static _wstring		RemoveWhiteSpace(const _wstring& _str);
	static SQLDATATYPE		String2DataType(const _tchar* _str, OUT _int& _maxLen);
};

/*
===============================
USqlHelpers
===============================
USqlColumn
===============================
*/

/*
@ Date: 2024-03-25, Writer: 박태현
@ Explain: 데이터베이스의 세로 값을 의미한다.
*/
class USqlColumn {
public:
	USqlColumn() :
		m_wstrName{ L"" }, m_iColumnID{ 0 }, m_SqlDataType{ SQLDATATYPE::DATATYPE_END },
		m_TypeString{ L"" }, m_iMaxLength{ 0 }, m_isNullable{ false }, m_isIdentity{ false },
		m_SeedValue{ 0 }, m_IncrementValue{ 0 }, m_wstrDefaultName{ L"" }, m_wstrDefaultConstraintName{ L"" }
	{}
public:
	_wstring CreateText();
public: /* get*/
	const _wstring& GetName() const { return m_wstrName; }
	_int GetColumnID() const { return m_iColumnID; }
	SQLDATATYPE GetSqlDataType() const { return m_SqlDataType; }
	const _wstring& GetTypeString() const { return m_TypeString; }
	_int GetMaxLength() const { return m_iMaxLength; }
	_llong GetSeedValue() const { return m_SeedValue; }
	_llong GetIncrementValue() const { return m_IncrementValue; }
	const _wstring& GetDefaultName() const { return m_wstrDefaultName; }
	const _wstring& GetDefaultConstrainName() const { return m_wstrDefaultConstraintName; }
public:

private:
	_wstring					m_wstrName;
	_int							m_iColumnID;
	SQLDATATYPE		m_SqlDataType;
	_wstring					m_TypeString;
	_int							m_iMaxLength;
	_bool						m_isNullable;
	_bool						m_isIdentity;
	_llong						m_SeedValue;
	_llong						m_IncrementValue;
	_wstring					m_wstrDefaultName;
	_wstring					m_wstrDefaultConstraintName;
};

/*
===============================
USqlColumn
===============================
UIndex
===============================
*/

class UIndex {
public:
	_wstring GetUniqueName();
	_wstring CreateName(const _wstring& _wstrTableName);
	_wstring GetTypeText();
	_wstring GetKeyText();
	_wstring CreateColumnsText();
	_bool IsDependsOn(const _wstring& _wstrColumnName);

private:
	_wstring													m_wstrName;
	_int															m_iIndexID;
	SQLINDEXTYPE									m_SqlIndexType;
	_bool														m_isPrimaryKey;
	_bool														m_isUniqueConstraint;
	CONVECTOR<USqlColumnRef>		m_spSqlColumRef;
};

END