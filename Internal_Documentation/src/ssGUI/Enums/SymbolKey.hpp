#ifndef SSGUI_SYMBOL_KEY
#define SSGUI_SYMBOL_KEY

//namespace: ssGUI::Enums
namespace ssGUI::Enums
{
    /*enum: SymbolKey

    BACK_QUOTE              - Back quote key. (Also know as Tilde key)
    MINUS                   - Minus key (-) 
    EQUAL                   - Equal key (=)
    LEFT_BRACKET            - Left bracket key ({)
    RIGHT_BRACKET           - Right bracket key (})
    BACKSLASH               - Backslash key (\)
    SEMICOLON               - Semicolon key (;)
    QUOTE                   - Quote key (')
    HASH_UK                 - Hash key for UK keyboard layout (#) (SFML backend doesn't support it)
    BACKSLASH_UK            - Backslash key for UK keyboard layout (\) (SFML backend doesn't support it)
    COMMA                   - Comma key (,)
    PERIOD                  - Period key (.)
    FORWARD_SLASH           - Forward slash key (/)
    NUMPAD_FORWARD_SLASH    - Forward slash key for numpad (/)
    NUMPAD_MULTIPLY         - Multiply key for numpad (*)
    NUMPAD_MINUS            - Minus key for numpad (-)
    NUMPAD_PLUS             - Plus key for numpad (+)
    NUMPAD_PERIOD           - Period key for numpad (.)
    */
    enum class SymbolKey
    {
        BACK_QUOTE, MINUS, EQUAL, LEFT_BRACKET, RIGHT_BRACKET, BACKSLASH, SEMICOLON,
        QUOTE, HASH_UK, BACKSLASH_UK, COMMA, PERIOD, FORWARD_SLASH, NUMPAD_FORWARD_SLASH, NUMPAD_MULTIPLY,
        NUMPAD_MINUS, NUMPAD_PLUS, NUMPAD_PERIOD 
    };
}


#endif