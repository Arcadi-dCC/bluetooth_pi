#include <menu.h>

namespace menu{

    volatile uint8 input = 0U; //Registers buttons input. Set by interrupt, Reset by stateMgr()
    /*
    * 0 -> No new input
    * 1 -> Left button  & Short press
    * 2 -> Left button  & Long press
    * 3 -> Right button & Short press
    * 4 -> Right button & Long press
    */

    uint8 state = 0U; //Current state in the FSM register. In initial state by default

    /*
    * 0b0xxxxxxx -> time menu
    * 0b1xxxxxxx -> bluetooth menu
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    *
    */

    //Initializes menu FSM
    void config(void){
        state = 0U;
        
    }

    //Manages the switching between states in the menu FSM
    void stateMgr(void){

        input = 0U;
        //END:restore interrupt
    }

};