#include <fstream>
#include <iostream>
#include<string>

#include<common/GeneralException.h> // must be declared before DBException.

#include<dao/DAOConstants.h>
#include<dbaccess/ODBCConnection.h>
#include<dbaccess/ODBCResultSet.h>
#include<dbaccess/ODBCStatement.h>
#include<dbaccess/dbAccess.h>

#include<dbaccess/DBException.h>
#include<dbaccess/ODBCError.h>
#include<business/idgen/EmpIdGen.h>

#include<string>

/**@file EmpIdGen.h
* @brief Declares the EmpIdGen
*
* <BR>NAME:EmpIdGen
*
* <BR>BASE CLASSES:None
*
* <BR>PURPOSE:To generate employee id.
*
* <BR>AUTHOR:Smitha Manjunath
* <BR>
* <BR>$Revision: $12/14/2005
*
* <BR>$Log:12/14/2005
*
* <BR>COPYRIGHT NOTICE:
* <BR>Copyright (c) 2005 C++ Capability team at Accenture. All rights reserved.
*/

/**@namespace idgen
 *@brief All the classes related to IDGenerator is defined in this namespace.
 */

namespace idgen {

EmpIdGen* EmpIdGen::m_thisInstance = NULL;


/**@class EmpIdGen
* @brief  Declaration of EmpIdGen
* <PRE>This class shall do the following
* This class is the singleton.
* It will provide a method to get an instance of it.
* It will also provide a method to create a next id.
</PRE>
*/


 /**@fn EmpIdGen.
  * @brief empty private Constructor.
  * This constructor will not take any argument.
  * Instance can be obtained through getInstance()
  * @param none
  * @return none
  */
  EmpIdGen::EmplIdGen()
  {

  }


 /**@fn getInstance
  * @brief implements singleton EmplIdGen class.
  * @param none
  * @return Pointer to EmpIdGen.
  */
  static EmpIdGen* EmpIdGen::getInstance()
	{
		if (m_thisInstance == Null)
		{
			m_thisInstance = new EmplIdGen;
		}
		return m_thisInstance;
	}


 /**@fn getNextId
  * @brief gets the next system-generated id.
  * @param none.
  * @return string.
  */
  std::string EmpIdGen::getNextId()
	{
		 try{
      dbaccess::ODBCConnection* conn = dbaccess::DBAccess::getConnection();
      if(conn->getError().isError()) //Checks for error.
	{
	  throw new GeneralException(conn->getError().getErrorMessage());
	}
      dbaccess::ODBCStatement* stmt = conn->createStatement();
      if(conn->getError().isError()) //Checks for error.
	{
	  throw new GeneralException(conn->getError().getErrorMessage());
	}

      dbaccess::ODBCResultSet* res = stmt->executeQuery( DAOConstants::EMPSQL_GETID);
      if(conn->getError().isError()) //Checks for error.
	{
	  throw new GeneralException(conn->getError().getErrorMessage());
	}

      if(!res->next())
	{
	  res->close();
	  stmt->close();
	  dbaccess::DBAccess::releaseConnection();
	  throw new GeneralException("Error obtaining next Id");
	}

      std::string id = res->getString(1);

      res->close();
      stmt->close();

      dbaccess::DBAccess::releaseConnection();

      return id;

    }catch(dbaccess::DBException* dbe)
      {
	throw new GeneralException(dbe->getMessage());
      }
	}


}	//namespace idgen
#endif //EMPIDGEN_H
