/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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
#include "Threads/skCriticalSection.h"
#include "Threads/skProcessQueue.h"
#include "Threads/skThread.h"
#include "Utils/skDebugger.h"
#include "Utils/skSingleton.h"
#include "Utils/skString.h"
#include "gtest/gtest.h"

/// <summary>
///  Output manager to test the actual print out.
/// </summary>
class GlobalOutputManager : public skSingleton<GlobalOutputManager>
{
private:
    skString m_buffer;

public:
    GlobalOutputManager()  = default;
    ~GlobalOutputManager() override = default;

    void log(const skString& message)
    {
        m_buffer.append(message);
    }

    void expectEq(const skString& value) const
    {
        EXPECT_TRUE(m_buffer.equals(value));
    }

    void expectVaryingOutput(int atLeastN_Variations) const
    {
        // this test will pass if the lines alternate
        // to some extent. Its an unpredictable timing issue
        // when using wait to suspend the thread. So the expectation
        // is that there is some variation (IE: it is being suspended! ).

        skStringArray arr;
        m_buffer.split(arr, "\n");

        skString c, p;
        int      nVariations = 0;

        for (SKuint32 i=1; i<arr.size(); i+=2)
        {
        
            p = arr[i - 1];
            c = arr[i];

            p = p.substr(p.size() - 4, p.size());
            c = c.substr(c.size() - 4, c.size());

            if (!p.equals(c))
                nVariations++;
        }

        printf("nVariations=%i\n", nVariations);

        EXPECT_GE(nVariations, atLeastN_Variations);
    }

    SK_DECLARE_SINGLETON(GlobalOutputManager)
};

SK_IMPLEMENT_SINGLETON(GlobalOutputManager)

class TestThread : public skRunnable
{
private:
    skString m_name;

public:
    TestThread(const char* name)
    {
        m_name = name;
    }

    int update() override
    {
        printf("%s\n", m_name.c_str());
        return 0;
    }
};

GTEST_TEST(Thread1, Test1)
{
    TestThread a("Thread A"), b("Thread B");

    printf("--[-------------------------------------]--\n");
    printf("Starting A (%p)\n", (void*)&a);
    a.start();
    printf("Starting B (%p)\n", (void*)&b);
    b.start();

    printf("Stopping A (%p)\n", (void*)&a);
    a.join();
    printf("Stopping B (%p)\n", (void*)&b);
    b.join();

    printf("--[-------------------------------------]--\n");
    EXPECT_TRUE(true);
}

static skCriticalSection gsCritical;

class TestThreadSwitch : public skRunnable
{
private:
    skString m_name;
    int      m_color;

public:
    TestThreadSwitch(const char* name, int color)
    {
        m_color = color;
        m_name  = name;
    }

    int update() override
    {
        for (int i = 0; i < 5; ++i)
        {
            {
                SK_SCOPE_LOCK_CRITICAL_SECTION(&gsCritical);
                EXPECT_NE(GlobalOutputManager::getSingletonPtr(), nullptr);

                skDebugger::writeColor((skConsoleColorSpace)m_color);

                skString out = skString::format("Update %i in %s\n", i + 1, m_name.c_str());
                fprintf(stdout, "%s", out.c_str());
                fflush(stdout);

                GlobalOutputManager::getSingleton().log(out);
                skDebugger::writeColor((skConsoleColorSpace)CS_WHITE);
            }
            wait(1);
        }
        return 0;
    }
};

GTEST_TEST(Thread1, Test2)
{
    const GlobalOutputManager output;

    TestThreadSwitch a("A", CS_RED), b("B", CS_DARKMAGENTA);
    printf("--[-------------------------------------]--\n");
    printf("Starting A (%p)\n", (void*)&a);
    printf("Starting B (%p)\n", (void*)&b);


    a.start();
    b.start();
    a.join();
    b.join();

    printf("--[-------------------------------------]--\n");

    output.expectVaryingOutput(3);
    EXPECT_TRUE(true);
}

class TestPacket : public skPacket
{
public:
    TestPacket(SKint32 id) :
        skPacket(id)
    {
    }

    int handle() override
    {
        EXPECT_NE(GlobalOutputManager::getSingletonPtr(), nullptr);

        GlobalOutputManager::getSingleton().log(
            skString::format("Handle background packet %i\n",
                             getId()));
        printf("Handle background packet %i\n", getId());
        return 0;
    }
};

GTEST_TEST(Thread1, Test3)
{
    GlobalOutputManager output;
    skProcessQueue      queue;

    queue.start();
    queue.enqueue(new TestPacket(1));
    queue.enqueue(new TestPacket(2));
    queue.enqueue(new TestPacket(3));
    queue.enqueue(new TestPacket(4));
    queue.stop();

    EXPECT_NE(GlobalOutputManager::getSingletonPtr(), nullptr);

    GlobalOutputManager::getSingleton().expectEq(
        "Handle background packet 1\n"
        "Handle background packet 2\n"
        "Handle background packet 3\n"
        "Handle background packet 4\n");

    EXPECT_TRUE(true);
}
