#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H


namespace Core
{
	/*
	@ Date: 2024-01-09,  Writer: 박태현
	@ Explain:  LOGTYPE의 enum 
	*/
	enum class LOGTYPE : unsigned int
	{
		CONSOL,
		SAVE,
		// End 
		LOG_END,
	};

	namespace Sql
	{
		/*
		@ Date: 2024-03-24,  Writer: 박태현
		@ Explain:  MSSql에 존재하는 DataType, 각각 데이터에 대응하는 번호를 나타낸다. 
		*/
		enum class  DATATYPE
		{
			NONE = 0,
			TINYINT = 48,
			SMALLINT = 52,
			INT = 56,
			REAL = 59,
			DATETIME = 61,
			FLOAT = 62,
			BIT = 104,
			NUMERIC = 108,
			BIGINT = 127,
			VARBINARY = 165,
			VARCHAR = 167,
			BINARY = 173,
			NVARCHAR = 231,
			DATATYPE_END
		};
		/*
		@ Date: 2024-03-24,  Writer: 박태현
		@ Explain:  데이터를 어떻게 정렬할지 정하는 enum 
		*/
		enum class INDEXTYPE
		{
			CLUSTERED = 1,
			NONCLUSTERED = 2,
			INDEX_END
		};		
		/*
		@ Date: 2024-03-24,  Writer: 박태현
		@ Explain:  Table의 Type 
		*/
		enum class TABLETYPE
		{
			LOGIN, 
			TABLE_END
		};
		/*
		@ Date: 2024-03-31,  Writer: 박태현
		@ Explain:  Query Type을 정의한다. 
		*/
		enum class QUERYTYPE
		{
			INSERT, 
			UPDATE,
			REMOVE, 
			FIND, 
			QUERY_END
		};
	}

	using SQLDATATYPE = Sql::DATATYPE;
	using SQLINDEXTYPE = Sql::INDEXTYPE;
	using SQLTABLETYPE = Sql::TABLETYPE;
	using SQLQUERYTYPE = Sql::QUERYTYPE;

	enum class SESSIONTYPE : unsigned int {
		OBJECT, PLAYER, MONSTER, TYPE_END
	};

	enum class SERVICETYPE { CLIENT, SERVER, TYPE_END };

	enum class COLLIDERTYPE {
		AABB, OBB, SPHERE, COLL_END
	};


	enum {
		MAX_BUFFER_LENGTH = 512, 
		MAX_PROCESSBUF_LENGTH = MAX_BUFFER_LENGTH * 5,
		MAX_OCTREENODE_LENGTH = 8, 
		MAX_SQLSTRING_LENGTH = 200,
	};
}



#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H