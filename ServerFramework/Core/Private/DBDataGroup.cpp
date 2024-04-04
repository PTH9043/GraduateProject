#include "CoreDefines.h"
#include "DBDataGroup.h"

namespace Core
{
	// Format을 리턴
	_string USqlHelpers::Format(const _char* _format, ...)
	{
		_char buf[4096];

		va_list ap;
		va_start(ap, _format);
		::printf_s(buf, 4096, _format, ap);
		va_end(ap);

		return _string(buf);
	}

	// 데이터 타입을 String 형태로 리턴
	_string USqlHelpers::DataType2String(SQLDATATYPE _type)
	{
		switch (_type)
		{
		case SQLDATATYPE::TINYINT:		return "TinyInt";
		case SQLDATATYPE::SMALLINT:	return "SmallInt";
		case SQLDATATYPE::INT:			return "Int";
		case SQLDATATYPE::REAL:		return "Real";
		case SQLDATATYPE::DATETIME:	return "DateTime";
		case SQLDATATYPE::FLOAT:		return "Float";
		case SQLDATATYPE::BIT:			return "Bit";
		case SQLDATATYPE::NUMERIC:		return "Numeric";
		case SQLDATATYPE::BIGINT:		return "BigInt";
		case SQLDATATYPE::VARBINARY:	return "VarBinary";
		case SQLDATATYPE::VARCHAR:		return "Varchar";
		case SQLDATATYPE::BINARY:		return "Binary";
		case SQLDATATYPE::NVARCHAR:	return "NVarChar";
		default:					
			return "None";
		}
	}

	_string USqlHelpers::RemoveWhiteSpace(const _string& _str)
	{
		_string ret = _str;

		ret.erase(
			std::remove_if(ret.begin(), ret.end(), [=](_char ch) { return ::isspace(ch); }),
			ret.end());

		return ret;
	}

	// std::wregex: 넓은 문자를 사용하는 정규
	//  표현식을 처리하기 위한 클래스로 문자열에서 정규 표현식을 사용하여 검색하고 매칭하는 기능 제공

	// std::wcmatch: 정규 표현식과 일치하는 문자열의 각 부분은 wcmath 객체에 저장된다. 

	SQLDATATYPE USqlHelpers::String2DataType(const _char* _str, OUT _int& _maxLen)
	{
		// 정규식 표현 설정 
		std::regex reg("([a-z]+)(\\((max|\\d+)\\))?");
		std::cmatch ret;

		if (false == std::regex_match(_str, OUT ret, reg))
		{
			return SQLDATATYPE::NONE;
		}

		if (true == ret[3].matched)
		{
			_maxLen = ::_strcmpi(ret[3].str().c_str(), "max") == 0 ? -1 : atoi(ret[3].str().c_str());
		}
		else
			_maxLen = 0;

		if (::_strcmpi(ret[1].str().c_str(), "TinyInt") == 0) return SQLDATATYPE::TINYINT;
		if (::_strcmpi(ret[1].str().c_str(), "SmallInt") == 0) return SQLDATATYPE::SMALLINT;
		if (::_strcmpi(ret[1].str().c_str(), "Int") == 0) return SQLDATATYPE::INT;
		if (::_strcmpi(ret[1].str().c_str(), "Real") == 0) return SQLDATATYPE::REAL;
		if (::_strcmpi(ret[1].str().c_str(), "DateTime") == 0) return SQLDATATYPE::DATETIME;
		if (::_strcmpi(ret[1].str().c_str(), "Float") == 0) return SQLDATATYPE::FLOAT;
		if (::_strcmpi(ret[1].str().c_str(), "Bit") == 0) return SQLDATATYPE::BIT;
		if (::_strcmpi(ret[1].str().c_str(), "Numeric") == 0) return SQLDATATYPE::NUMERIC;
		if (::_strcmpi(ret[1].str().c_str(), "BigInt") == 0) return SQLDATATYPE::BIGINT;
		if (::_strcmpi(ret[1].str().c_str(), "VarBinary") == 0) return SQLDATATYPE::VARBINARY;
		if (::_strcmpi(ret[1].str().c_str(), "Varchar") == 0) return SQLDATATYPE::VARCHAR;
		if (::_strcmpi(ret[1].str().c_str(), "Binary") == 0) return SQLDATATYPE::BINARY;
		if (::_strcmpi(ret[1].str().c_str(), "NVarChar") == 0) return SQLDATATYPE::NVARCHAR;

		return SQLDATATYPE::NONE;
	}

	/*
	===============================
	USqlHelpers
	===============================
	USqlColumn
	===============================
	*/


	_wstring USqlColumn::CreateText(){
		return L"";
	}

	/*
	===============================
	USqlColumn
	===============================
	USqlIndex
	===============================
	*/

	_wstring UIndex::GetUniqueName()
	{
		return _wstring();
	}

	_wstring UIndex::CreateName(const _wstring& _wstrTableName)
	{
		return _wstring();
	}

	_wstring UIndex::GetTypeText()
	{
		return _wstring();
	}

	_wstring UIndex::GetKeyText()
	{
		return _wstring();
	}

	_wstring UIndex::CreateColumnsText()
	{
		return _wstring();
	}

	_bool UIndex::IsDependsOn(const _wstring& _wstrColumnName)
	{
		return _bool();
	}

}