#include "Connection.h"

void main()
{
	try
	{
		std::string phrase = "test pentru testarea unei baze de date.test de date?date!";

		Connection* myConn = new Connection();
		myConn->managePhrase(phrase);


	}
	catch (sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line "
		<< __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

}