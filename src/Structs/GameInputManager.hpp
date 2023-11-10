#pragma once

class GameInputManager
{
    public:
        virtual void VFunction0();
        virtual void VFunction8();
        virtual void VFunction16();
        virtual void VFunction24();
        virtual void VFunction32();
        virtual void VFunction40();
        virtual void VFunction48();
        virtual void VFunction56();
        virtual void VFunction64();
        virtual void VFunction72();
        virtual void VFunction80();
        virtual void VFunction88();
        virtual void VFunction96();
        virtual void SendCommand(WORD* return_value, int* command_struct);
};
