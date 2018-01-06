#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iosfwd>

#include "../Parser/Parser.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/ConfigureParser.h"
#include "../Utilities/Utilities.h"

namespace CodeAnalysis
{
	using Utils = Utilities::StringHelper;

	///////////////////////////////////////////////////////////////////
	// AnalFileMgr class derives from FileManager::FileMgr
	// - provides application specific handling of file and dir events

	class AnalFileMgr : public FileManager::FileMgr
	{
	public:
		using Path = std::string;
		using File = std::string;
		using Files = std::vector<File>;
		using Pattern = std::string;
		using Ext = std::string;
		using FileMap = std::unordered_map<Pattern, Files>;

		AnalFileMgr(const Path& path, FileMap& fileMap);
		virtual void file(const File& f);
		virtual void dir(const Dir& d);
		virtual void done();
		size_t numFiles();
		size_t numDirs();
	private:
		Path d_;
		FileMap& fileMap_;
		bool display_;
		size_t numFiles_;
		size_t numDirs_;
	};

	///////////////////////////////////////////////////////////////////
	// CodeAnalysisExecutive class directs C++ and C# code analysis

	class CodeAnalysisExecutive
	{
	public:
		using Path = std::string;
		using Pattern = std::string;
		using Patterns = std::vector<Pattern>;
		using File = std::string;
		using Files = std::vector<File>;
		using Ext = std::string;
		using Options = std::vector<char>;
		using FileMap = std::unordered_map<Pattern, Files>;
		using FileNodes = std::vector<std::pair<File, ASTNode*>>;
		using Slocs = size_t;
		using SlocMap = std::unordered_map<File, Slocs>;

		CodeAnalysisExecutive();
		virtual ~CodeAnalysisExecutive();

		CodeAnalysisExecutive(const CodeAnalysisExecutive&) = delete;
		CodeAnalysisExecutive& operator=(const CodeAnalysisExecutive&) = delete;

		void showCommandLineArguments(int argc, char* argv[]);
		bool ProcessCommandLine(int argc, char* argv[]);
		FileMap& getFileMap();
		std::string getAnalysisPath();
		virtual void getSourceFiles();
		virtual void processSourceCode(bool showActivity);
		void complexityAnalysis();
		std::vector<File>& cppHeaderFiles();
		std::vector<File>& cppImplemFiles();
		std::vector<File>& csharpFiles();
		Slocs fileSLOCs(const File& file);
		size_t numFiles();
		size_t numDirs();
		std::string systemTime();
		virtual void displayMetrics(ASTNode* root);
		virtual void displayMetrics();
		virtual void displayMetricSummary(size_t sMax, size_t cMax);
		virtual void displaySlocs();
		virtual void displayAST();
		virtual void dispatchOptionalDisplays();
		virtual void setDisplayModes();
		void startLogger(std::ostream& out);
		void flushLogger();
		void stopLogger();
		void setLogFile(const File& file);
	private:
		void setLanguage(const File& file);
		void showActivity(const File& file);
		void clearActivity();
		virtual void displayHeader();
		virtual void displayMetricsLine(const File& file, ASTNode* pNode);
		virtual void displayDataLines(ASTNode* pNode, bool isSummary = false);
		std::string showData(const Scanner::ITokCollection* ptc);
		Parser* pParser_;
		ConfigParseForCodeAnal configure_;
		Repository* pRepo_;
		Path path_;
		Patterns patterns_;
		Options options_;
		FileMap fileMap_;
		FileNodes fileNodes_;
		std::vector<File> cppHeaderFiles_;
		std::vector<File> cppImplemFiles_;
		std::vector<File> csharpFiles_;
		size_t numFiles_;
		size_t numDirs_;
		SlocMap slocMap_;
		bool displayProc_ = false;
		std::ofstream* pLogStrm_ = nullptr;
	};
}
