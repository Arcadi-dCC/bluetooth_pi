#include <menu.h>

namespace menu{

    enum Input{
        NOTHING,
        LEFT_SHORT,
        LEFT_LONG,
        LEFT_RELEASE,
        RIGHT_SHORT,
        RIGHT_LONG,
        RIGHT_RELEASE
    };

    enum State{
        AY,
        BEY
    }; //Current state in the FSM register. In initial state by default

    volatile Input input;
    State state;

    //Initializes menu FSM
    void config(void){
        state = AY;
        
    }

    //Manages the switching between states in the menu FSM
    void stateMgr(void){

        switch(state){
            case AY:{
                break;
            }
        }

        input = NOTHING;
        //END:restore interrupt
    }

};