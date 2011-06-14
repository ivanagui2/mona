/*
 *   Copyright (c) 2011  Higepon(Taro Minowa)  <higepon@users.sourceforge.jp>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <monagui.h>
#define MUNIT_GLOBAL_VALUE_DEFINED
#include <monapi/MUnit.h>
#include "Terminal.h"
#include "TestTerminal.h"
#include <monapi/Robot.h>

using namespace MonAPI;

class TerminalOutputProbe : public Probe
{
private:
    TestTerminal& terminal_;
    std::string lastContent_;
    std::string contentToMatch_;
public:
    TerminalOutputProbe(TestTerminal& terminal, const std::string& contentToMatch) : 
        terminal_(terminal),
        contentToMatch_(contentToMatch)
    {
    }

    void sample()
    {
        lastContent_ = terminal_.getOutput();
    }
    bool isSatisfied()
    {
        return lastContent_.find(contentToMatch_) != std::string::npos;
    }

    void describeTo(std::string& d)
    {
        d += "<";
        d += contentToMatch_;
        d += ">";
    }

    void describeFailureTo(std::string& d)
    {
        d += "<";
        d += lastContent_;
        d += "> ";
    }
};

class TerminalLastLineProbe : public Probe
{
private:
    TestTerminal& terminal_;
    std::string lastContent_;
public:
    TerminalLastLineProbe(TestTerminal& terminal) : 
        terminal_(terminal)
    {
    }

    void sample()
    {
        lastContent_ = terminal_.getOutput();
    }
    bool isSatisfied()
    {
        return lastContent_.find(terminal_.getLastLine()) != std::string::npos;
    }

    void describeTo(std::string& d)
    {
        d += "<>";
    }

    void describeFailureTo(std::string& d)
    {
        d += "<";
        d += lastContent_;
        d += "> ";
    }
};


static Stream* outStream;
static Terminal* terminal;
static std::string sharedString;

static void __fastcall stdoutStreamReader(void* mainThread)
{
    scoped_array<char> buf(new char[outStream->capacity()]);

    // read from outStream, accumulates as string.
    // Then notifies data has come.
    for (;;) {
        logprintf("%s %s:%d\n", __func__, __FILE__, __LINE__);
        uint32_t sizeRead = outStream->read(buf.get(), outStream->capacity(), true);
        logprintf("%s %s:%d\n", __func__, __FILE__, __LINE__);
        sharedString.clear();
        sharedString += std::string(buf.get(), sizeRead);
        MessageInfo info;
        if (Message::sendReceive(&info, (uint32_t)mainThread, MSG_UPDATE) != M_OK) {
            monapi_fatal("main thread is dead?");
        }
    }
}

class MonaGUIRobot : public Robot
{
public:
    void click(Component& button)
    {
        Rectangle r = *(button.getBounds());
        Frame* parent = (Frame*)button.getParent();
        ASSERT(parent);
        r.x += parent->getBounds()->x + parent->getInsets()->left;
        r.y += parent->getBounds()->y + parent->getInsets()->top;
        mouseMove(r.x, r.y);
        sleep(1000);
        mousePress();
        mouseRelease();
    }

    void clearInput(TextField& input)
    {
        input.setText("");
    }

    void input(Component& input, const std::string& text)
    {
        Rectangle r = *(input.getBounds());
        Frame* parent = (Frame*)input.getParent();
        ASSERT(parent);
        r.x += parent->getBounds()->x + parent->getInsets()->left;
        r.y += parent->getBounds()->y + parent->getInsets()->top;
        mouseMove(r.x, r.y);
        sleep(100);
        mousePress();
        mouseRelease();
        type(text.c_str());
    }
};

static uintptr_t waitSubThread(uintptr_t id)
{
    MessageInfo dst, src;
    src.header = MSG_STARTED;
    src.from = id;
    int ret = Message::receive(&dst, &src, Message::equalsFromHeader);
    ASSERT_EQ(M_OK, ret);
    uintptr_t tid = dst.from;
    return tid;
}

static void stopSubThread(uintptr_t id)
{
    ASSERT_EQ(M_OK, Message::send(id, MSG_STOP));
}

TestTerminal* testTerminal;

static void __fastcall testTerminalThread(void* arg)
{
    uintptr_t mainThread = System::getThreadID();
    uintptr_t hoge = monapi_thread_create_with_arg(stdoutStreamReader, (void*)mainThread);
    uintptr_t mainThread2 = (uintptr_t)arg;
    outStream = new Stream;
    testTerminal = new TestTerminal(mainThread2, *outStream, sharedString);
    testTerminal->run();
    delete testTerminal;
}

static void testLSCommandReturnsLFSeperatedListOfFiles()
{
    MonaGUIRobot r;
    r.click(testTerminal->getButton());
    TerminalOutputProbe probe(*testTerminal, "HELLO.EX5");
    ASSERT_EVENTUALLY(probe);
}

static void testPSShowsHeaderAndProcess()
{
    MonaGUIRobot r;
    r.clearInput(testTerminal->getCommandField());
    r.input(testTerminal->getCommandField(), "ps");
    r.click(testTerminal->getButton());
    TerminalOutputProbe probe(*testTerminal, "tid name");
    ASSERT_EVENTUALLY(probe);
}

static void testLSCausesScrollToTheLastLine()
{
    MonaGUIRobot r;
    r.clearInput(testTerminal->getCommandField());
    r.input(testTerminal->getCommandField(), "ls /APPS/");
    r.click(testTerminal->getButton());
    TerminalLastLineProbe probe(*testTerminal);
    ASSERT_EVENTUALLY(probe);
}



    // void testEnterKeyDownRunsLSCommand()
    // {
    //     clearOutput();
    //     enterCommand("ls /APPS/");
    //     EXPECT_TRUE(find(lines_.begin(), lines_.end(), "TEST.RAW") != lines_.end());
    // }

    // void testLSCausesScrollToTheLastLine()
    // {
    //     clearOutput();
    //     command_->setText("ls /APPS/");
    //     buttonClick();
    //     // On the output, we should include the last line.
    //     EXPECT_TRUE(strstr(output_->getText(), lines_[lines_.size() - 1].c_str()) != NULL);
    // }


    // void testCommandEnteredAppearsOnHistory()
    // {
    //     clearOutput();
    //     enterCommand("ls /LIBS/");
    //     enterCommand("ls /USER/");

    //     pressCtrlKey('p');
    //     EXPECT_EQ(0, strcmp("ls /LIBS/", command_->getText()));

    //     pressCtrlKey('n');
    //     EXPECT_EQ(0, strcmp("ls /USER/", command_->getText()));
    // }




static void test()
{
    testLSCommandReturnsLFSeperatedListOfFiles();
    testPSShowsHeaderAndProcess();
    testLSCausesScrollToTheLastLine();
    TEST_RESULTS();
}

// remove unused
// extract test function
// refactor
int main(int argc, char* argv[])
{
    intptr_t ret = monapi_enable_stacktrace("/APPS/MONAGUI/TERMINAL.MAP");
    if (ret != M_OK) {
        fprintf(stderr, "terminal: stack_trace_enable failed error=%d %d.\n", ret, syscall_get_tid());
    }
    bool isTestMode = false;
    if (argc == 2 && strcmp(argv[1], "-t") == 0) {
        isTestMode = true;
    }

    outStream = new Stream;
    uintptr_t mainThread = System::getThreadID();
    if (isTestMode) {
        uint32_t id = monapi_thread_create_with_arg(testTerminalThread, (void*)mainThread);
        waitSubThread(id);
        test();
        stopSubThread(id);
    } else {
        terminal = new Terminal(*outStream, sharedString);
        monapi_thread_create_with_arg(stdoutStreamReader, (void*)mainThread);
//        Rectangle r = terminal->getButtonAbsoluteBounds();
//        logprintf("r.x=%d r.y=%d", r.x, r.y);
        terminal->run();
        delete outStream;
        delete terminal;
        return 0;
    }
}
