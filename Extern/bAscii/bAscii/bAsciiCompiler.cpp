/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "bAsciiCompiler.h"
#include <cstdio>
#include <cstdlib>
#include "bAscii.h"
#include "bAsciiArgument.h"
#include "bAsciiBlock.h"
#include "bAsciiKeywords.h"
#include "bAsciiScanner.h"

#define LOG_SCAN(op, tok)                  \
    {                                      \
        printf("\t==> 0x%02X %s\n", \
               (unsigned char)(op),        \
               (tok).string.c_str());      \
    }

bAsciiCompiler::bAsciiCompiler(bAscii* parent, bool verbose) :
    m_parent(parent),
    m_verbose(verbose)
{
}

bool bAsciiCompiler::compileBlock(bAsciiScanner&     scanner,
                                  const bAsciiLocal* globals,
                                  const bAsciiLocal* locals,
                                  bAsciiBlock*       parent) const
{
    // find extra arguments
    bAsciiToken tok;

    bCode op = scanner.scan(tok, m_verbose);
    if (m_verbose)
        LOG_SCAN(op, tok)

    if (op != OP_SECTION)
    {
        printf("%s(%i): expecting sub section ':', found '%s' \n",
               scanner.getSource(),
               tok.line,
               tok.string.c_str());
        return false;
    }

    // look for optional sub section name|number

    op = scanner.scan(tok, m_verbose);
    if (m_verbose)
        LOG_SCAN(op, tok)

    if (locals && locals->ctor != OP_UNKNOWN)
    {
        if (op != locals->ctor)
        {
            const char* dbg = locals->ctor == OP_STRING ? "string" : "number";

            printf("%s(%i): missing block constructor expected '%s', found '%s' \n",
                   scanner.getSource(),
                   tok.line,
                   dbg,
                   tok.string.c_str());
            return false;
        }
    }

    if (op == OP_STRING)
    {
        parent->setName(tok.string);
        m_parent->addBlock(tok.string, parent);
        op = scanner.scan(tok, m_verbose);

        if (m_verbose)
            LOG_SCAN(op, tok)
    }
    else if (op == OP_NUMBER)
    {
        parent->setLength(skChar::toInt32(tok.string));
        op = scanner.scan(tok, m_verbose);
        if (m_verbose)
            LOG_SCAN(op, tok)
    }

    // assert
    if (op != OP_BRACKET_OPEN)
    {
        printf("%s(%i): expecting open bracket '{', found '%s' (%d) \n",
               scanner.getSource(),
               tok.line,
               tok.string.c_str(),
               op);
        return false;
    }

    // scan recursively
    do
    {
        op = scanner.scan(tok, m_verbose);
        if (m_verbose)
            LOG_SCAN(op, tok)

        if (op == OP_BRACKET_CLOSE)
            break;

        if (op != OP_ALPHA)
        {
            printf("%s(%i): expecting a block or statement, found '%s' \n",
                   scanner.getSource(),
                   tok.line,
                   tok.string.c_str());
            return false;
        }

        const bAsciiLocal* local;
        if (locals)
            local = bAsciiFindLocal(locals, skHash(tok.string));
        else
            local = bAsciiFindLocal(globals, skHash(tok.string));

        if (!local)
        {
            printf("%s(%i): invalid keyword '%s' \n",
                   scanner.getSource(),
                   tok.line,
                   tok.string.c_str());
            return false;
        }

        if (local->types)
        {
            // it's a sub block
            bAsciiBlock* block = new bAsciiBlock(tok.string, local->op);
            block->setParent(parent);
            m_parent->addBlock(block);

            if (!compileBlock(scanner, globals, local, block))
                return false;
        }
        else
        {
            const bString cTok = tok.string;

            op = scanner.scan(tok, m_verbose);
            if (m_verbose)
                LOG_SCAN(op, tok)

            // its and assignment

            if (tok.tok != OP_ASSIGNMENT)
            {
                printf("%s(%i): expecting assignment statement, found '%s' (%d)\n",
                       scanner.getSource(),
                       tok.line,
                       tok.string.c_str(),
                       tok.tok);
                return false;
            }

            op = scanner.scan(tok, m_verbose);
            if (m_verbose)
                LOG_SCAN(op, tok)

            if (op == OP_TERM)
            {
                printf("%s(%i): empty assignment statement.\n",
                       scanner.getSource(),
                       tok.line);
                return false;
            }

            if (op == OP_END)
            {
                printf("%s(%i): end of file found while scanning assignment statement.\n",
                       scanner.getSource(),
                       tok.line);
                return false;
            }

            bAsciiArgument* arg = new bAsciiArgument(local->op, local->type, local->argCount);
            parent->addArgument(arg);

            int argNr       = 0;
            int argsToCheck = local->argCount;

            if (argsToCheck != -1 && local->type > OP_TYPE_ARRAY_BEGIN && local->type < OP_TYPE_ARRAY_END)
            {
                // nrArg then becomes the stride
                const int pLen = (int)parent->getLength();
                if (pLen != -1)
                    argsToCheck *= pLen;
                else
                    argsToCheck = -1;
            }

            // scan till term
            do
            {
                if (argsToCheck != -1 && op != OP_TERM && op != OP_SEPARATOR)
                {
                    if (argNr > argsToCheck)
                    {
                        const char* dbg = local->arg == OP_STRING ? "string(s)" : "number(s)";

                        printf("%s(%i): incorrect number of arguments to '%s', expected '%i' %s.\n",
                               scanner.getSource(),
                               tok.line,
                               cTok.c_str(),
                               argsToCheck,
                               dbg);
                        return false;
                    }
                }

                if (op == OP_STRING || op == OP_ALPHA)
                {
                    if (local->arg != OP_STRING && local->type != OP_TYPE_MIXED)
                    {
                        printf("%s(%i): invalid string argument to '%s', expected %i number(s).\n",
                               scanner.getSource(),
                               tok.line,
                               cTok.c_str(),
                               argsToCheck);
                        return false;
                    }
                }

                if (op == OP_NUMBER)
                {
                    if (local->arg != OP_NUMBER && local->type != OP_TYPE_MIXED)
                    {
                        printf("%s(%i): invalid string argument to '%s', expected %i string(s).\n",
                               scanner.getSource(),
                               tok.line,
                               cTok.c_str(),
                               argsToCheck);
                        return false;
                    }
                }

                if (op == OP_STRING || op == OP_ALPHA)
                {
                    arg->strings.push_back(tok.string);
                    argNr++;
                }
                else if (op == OP_NUMBER)
                {
                    arg->numbers.push_back(skChar::toFloat(tok.string));
                    argNr++;
                }
                else
                {
                    if (op != OP_TERM && op != OP_SEPARATOR)
                    {
                        printf("%s(%i): expecting ',' or ';', found '%s' \n",
                               scanner.getSource(),
                               tok.line,
                               tok.string.c_str());
                        return false;
                    }

                    if (op == OP_END)
                    {
                        printf("%s(%i): end of file found while scanning assignment statement.\n",
                               scanner.getSource(),
                               tok.line);
                        return false;
                    }
                }

                op = scanner.scan(tok, m_verbose);
                if (m_verbose)
                    LOG_SCAN(op, tok)

                if (op == OP_END)
                {
                    printf("%s(%i): end of file found while scanning assignment statement.\n",
                           scanner.getSource(),
                           tok.line);
                    return false;
                }

            } while (op != OP_TERM);

            if (local->maxArgCount != -1 && argNr > local->maxArgCount)
            {
                printf("%s(%i): maximum arguments exceeded for %s, max is %i, found %i.\n",
                       scanner.getSource(),
                       tok.line,
                       cTok.c_str(),
                       local->maxArgCount,
                       argNr);
                return false;
            }

            if (argsToCheck != -1 && argNr != argsToCheck)
            {
                printf(
                    "%s(%i): the required number of arguments for '%s' is %i."
                    " found %i argument(s).\n",
                    scanner.getSource(),
                    tok.line,
                    cTok.c_str(),
                    argsToCheck,
                    argNr);
                return false;
            }
        }
    } while (op != OP_BRACKET_CLOSE && op != OP_END);
    return true;
}

bool bAsciiCompiler::compile(bAsciiScanner& scanner) const
{
    bAsciiToken tok;
    bCode       op;

    do
    {
        op = scanner.scan(tok, m_verbose);
        if (m_verbose)
            LOG_SCAN(op, tok)

        if (op == OP_END)
            break;

        if (op == OP_UNKNOWN)
        {
            printf("%s(%i): unknown error, '%s' \n",
                   scanner.getSource(),
                   tok.line,
                   tok.string.c_str());
            return false;
        }

        // looking for top level start block OP_ALPHA
        if (op != OP_ALPHA)
        {
            printf("%s(%i): expecting start block, found '%s' \n",
                   scanner.getSource(),
                   tok.line,
                   tok.string.c_str());
            return false;
        }

        const bAsciiLocal* global = bAsciiFindGlobal(skHash(tok.string));
        if (!global || global->op == OP_UNKNOWN)
        {
            printf("%s(%i): unknown block '%s' \n",
                   scanner.getSource(),
                   tok.line,
                   tok.string.c_str());
            return false;
        }

        bAsciiBlock* block = new bAsciiBlock(tok.string, global->op);
        m_parent->addBlock(block);
        m_parent->addBlock(global->op, block);

        // build recursively
        if (!compileBlock(scanner, global, nullptr, block))
            return false;

    } while (op != OP_UNKNOWN && op != OP_END);
    return true;
}
