#pragma once
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <vector>

class Connection
{
	sql::Driver * driver;
	sql::Connection *con;
	
	sql::ResultSet *res;

	//split phrase in sentences
	std::vector<std::string> sentences;

	//each sentence will have a score
	std::map<std::string, int> scoreOfSentences;

	//each word from the phrase with its score
	std::map<std::string, int> wordsScore;

	//each word in phrase has a base word in DEX
	std::map<std::string, std::string> word_baseWord;

public:
	Connection();

	void setDatabase(std::string database);

	//split into senteces, score every word and after that score the sentence
	void managePhrase(std::string phrase);
	
	//populate the -sentences- vector 
	void splitIntoSentences(std::string phrase);

	//split sentence in words and after searching the word in DEX, check if the base word exists in a local map of words with their score
	void manageWordsFromSentence(std::string sentence);

	//score each sentence
	int scoreSentence(std::string sentence);

	//use this to search through DEX in inflectedForm & lexem and return the base word
	std::string baseWordFromDEX(std::string word);

	~Connection();

};