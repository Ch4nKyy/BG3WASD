#pragma once

class CommandHandler
{
    public:
        virtual void VFunction0();
        virtual void VFunction8();
        virtual void VFunction16();
        virtual void VFunction24();
        virtual void VFunction32();
        virtual void VFunction40();
        virtual void VFunction48();
        virtual void HandleCommand(WORD* return_value, Command* command);
};
