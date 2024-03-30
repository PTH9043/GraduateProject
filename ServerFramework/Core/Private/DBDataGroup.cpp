#include "CoreDefines.h"
#include "DBDataGroup.h"

namespace Core
{
	// Format을 리턴
	_wstring USqlHelpers::Format(const _tchar* _format, ...)
	{
		_tchar buf[4096];

		va_list ap;
		va_start(ap, _format);
		::vswprintf_s(buf, 4096, _format, ap);
		va_end(ap);

		return _wstring(buf);
	}

	// 데이터 타입을 String 형태로 리턴
	_wstring USqlHelpers::DataType2String(SQLDATATYPE _type)
	{
		switch (_type)
		{
		case SQLDATATYPE::TINYINT:		return L"TinyInt";
		case SQLDATATYPE::SMALLINT:	return L"SmallInt";
		case SQLDATATYPE::INT:			return L"Int";
		case SQLDATATYPE::REAL:		return L"Real";
		case SQLDATATYPE::DATETIME:	return L"DateTime";
		case SQLDATATYPE::FLOAT:		return L"Float";
		case SQLDATATYPE::BIT:			return L"Bit";
		case SQLDATATYPE::NUMERIC:		return L"Numeric";
		case SQLDATATYPE::BIGINT:		return L"BigInt";
		case SQLDATATYPE::VARBINARY:	return L"VarBinary";
		case SQLDATATYPE::VARCHAR:		return L"Varchar";
		case SQLDATATYPE::BINARY:		return L"Binary";
		case SQLDATATYPE::NVARCHAR:	return L"NVarChar";
		default:					
			return L"None";
		}
	}

	_wstring USqlHelpers::RemoveWhiteSpace(const _wstring& _str)
	{
		_wstring ret = _str;

		ret.erase(
			std::remove_if(ret.begin(), ret.end(), [=](_tchar ch) { return ::isspace(ch); }),
			ret.end());

		return ret;
	}

	// std::wregex: 넓은 문자를 사용하는 정규
	//  표현식을 처리하기 위한 클래스로 문자열에서 정규 표현식을 사용하여 검색하고 매칭하는 기능 제공

	// std::wcmatch: 정규 표현식과 일치하는 문자열의 각 부분은 wcmath 객체에 저장된다. 

	SQLDATATYPE USqlHelpers::String2DataType(const _tchar* _str, OUT _int& _maxLen)
	{
		// 정규식 표현 설정 
		std::wregex reg(L"([a-z]+)(\\((max|\\d+)\\))?");
		std::wcmatch ret;

		if (false == std::regex_match(_str, OUT ret, reg))
		{
			return SQLDATATYPE::NONE;
		}

		if (true == ret[3].matched)
		{
			_maxLen = ::_wcsicmp(ret[3].str().c_str(), L"max") == 0 ? -1 : _wtoi(ret[3].str().c_str());
		}
		else
			_maxLen = 0;


		if (::_wcsicmp(ret[1].str().c_str(), L"TinyInt") == 0) return SQLDATATYPE::TINYINT;
		if (::_wcsicmp(ret[1].str().c_str(), L"SmallInt") == 0) return SQLDATATYPE::SMALLINT;
		if (::_wcsicmp(ret[1].str().c_str(), L"Int") == 0) return SQLDATATYPE::INT;
		if (::_wcsicmp(ret[1].str().c_str(), L"Real") == 0) return SQLDATATYPE::REAL;
		if (::_wcsicmp(ret[1].str().c_str(), L"DateTime") == 0) return SQLDATATYPE::DATETIME;
		if (::_wcsicmp(ret[1].str().c_str(), L"Float") == 0) return SQLDATATYPE::FLOAT;
		if (::_wcsicmp(ret[1].str().c_str(), L"Bit") == 0) return SQLDATATYPE::BIT;
		if (::_wcsicmp(ret[1].str().c_str(), L"Numeric") == 0) return SQLDATATYPE::NUMERIC;
		if (::_wcsicmp(ret[1].str().c_str(), L"BigInt") == 0) return SQLDATATYPE::BIGINT;
		if (::_wcsicmp(ret[1].str().c_str(), L"VarBinary") == 0) return SQLDATATYPE::VARBINARY;
		if (::_wcsicmp(ret[1].str().c_str(), L"Varchar") == 0) return SQLDATATYPE::VARCHAR;
		if (::_wcsicmp(ret[1].str().c_str(), L"Binary") == 0) return SQLDATATYPE::BINARY;
		if (::_wcsicmp(ret[1].str().c_str(), L"NVarChar") == 0) return SQLDATATYPE::NVARCHAR;

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

	}
}