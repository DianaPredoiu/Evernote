#include "Connection.h"
#include <iostream>
#include <boost/algorithm/string.hpp>


Connection::Connection()
{
	/* Create a connection */
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
	con->setSchema("dex");
}

void Connection::setDatabase(std::string database)
{
	/* Connect to the MySQL test database */
	con->setSchema(database);
}

void Connection::splitIntoSentences(std::string phrase)
{

	boost::split(sentences, phrase, boost::is_any_of("./?!;"));

	//for (int i = 0; i < sentences.size(); i++)
	//	std::cout << sentences.at(i) << std::endl;
}

void Connection::managePhrase(std::string phrase)
{
	//setDatabase("dex");
	splitIntoSentences(phrase);

	for (int i = 0; i < sentences.size(); ++i)
	{
		if (!sentences.at(i).empty())
			manageWordsFromSentence(sentences.at(i));
	}

	for (int i = 0; i < sentences.size(); ++i)
	{
		scoreOfSentences[sentences.at(i)] = scoreSentence(sentences.at(i));
	}

	for (std::map<std::string, std::string>::iterator it = word_baseWord.begin(); it != word_baseWord.end(); ++it)
	{
		if (!it->first.empty())
			std::cout << it->first << " => " << it->second << '\n';
	}

	//for (std::map<std::string, int>::iterator it = wordsScore.begin(); it != wordsScore.end(); ++it)
	//{
	//	if (!it->first.empty())
	//		std::cout << it->first << " => " << it->second << '\n';
	//}

	//for (std::map<std::string, int>::iterator it = scoreOfSentences.begin(); it != scoreOfSentences.end(); ++it)
	//{
	//	if (!it->first.empty())
	//		std::cout << it->first << " => " << it->second << '\n';
	//}

}

int Connection::scoreSentence(std::string sentence)
{
	int score = 0;
	std::vector<std::string> words;
	boost::split(words, sentence, boost::is_any_of(" ,"));

	for (int i = 0; i < words.size(); ++i)
	{
		score += wordsScore[words.at(i)];
	}

	return score;
}

void Connection::manageWordsFromSentence(std::string sentence)
{
	std::vector<std::string> words;
	boost::split(words, sentence, boost::is_any_of(" ,"));

	std::string baseWord;
	std::map<std::string, int>::iterator it;

	for (int i = 0; i < words.size(); ++i)
	{
		baseWord = baseWordFromDEX(words.at(i));

		if (!baseWord.empty())
		{
			it = wordsScore.find(baseWord);
			if (it != wordsScore.end())
			{
				wordsScore[baseWord] ++;
			}
			else
			{
				wordsScore[baseWord] = 1;
			}
		}
	}

}

std::string Connection::baseWordFromDEX(std::string word)
{
	std::string originalWord = word;
	boost::algorithm::to_lower(word);

	std::map<std::string, std::string>::iterator it;
	sql::PreparedStatement* stmt;

	stmt = con->prepareStatement("select * from inflectedform where formNoAccent = ? ");
	stmt->setString(1, word.c_str());
	stmt->execute();

	res = stmt->getResultSet();

	if (res == NULL)
	{
		stmt = con->prepareStatement("select formNoAccent from lexem where formNoAccent = ? ");
		stmt->setString(1, word.c_str());
		stmt->execute();

		res = stmt->getResultSet();

		while (res->next())
		{
			it = word_baseWord.find(originalWord);
			if (it == word_baseWord.end())
				word_baseWord[originalWord] = res->getString("formNoAccent").c_str();
		}

	}
	else
	{
		int lexemID = -1;
		while (res->next())
		{
			lexemID = res->getInt("lexemId");
			stmt = con->prepareStatement("select formNoAccent from lexem where id = ? ");
			stmt->setInt(1, lexemID);
			stmt->execute();

			res = stmt->getResultSet();

			it = word_baseWord.find(originalWord);
			if (it == word_baseWord.end())
				word_baseWord[originalWord] = res->getString("formNoAccent").c_str();

			break;
		}

	}
	return word_baseWord[originalWord];

}

Connection::~Connection()
{
	delete res;
	delete con;
}


