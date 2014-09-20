/*
   @file
   @brief TODO

   @author John Bailey
   @copyright Copyright 2014 John Bailey

   @section LICENSE

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#if !defined( METRIC_SRC_LEXER_HPP )
#define       METRIC_SRC_LEXER_HPP

#include "MetricUnit.hpp"
#include "MetricOptions.hpp"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

class MetricSrcLexer
{
	protected:
		clang::CompilerInstance &m_compilerInstance;
		MetricUnit*		  m_topUnit;
		MetricOptions*    m_options;
		MetricUnit*       m_currentUnit;
		std::string       m_currentFileName;

		void CountToken( clang::Token& p_token );
		void CountLines( clang::StringRef& p_buffer );

	public:
		MetricSrcLexer(clang::CompilerInstance &p_CI, MetricUnit* p_topUnit,MetricOptions* p_options = NULL);
	    virtual ~MetricSrcLexer(void);

		void LexSources( clang::SourceManager& p_sm );
};

#endif     // !defined( METRIC_SRC_LEXER_HPP )