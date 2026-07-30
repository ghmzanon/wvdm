/* key bindings — edit to customize */

/* set to 0 to skip switch on move/new (removal always switches) */
#define SWITCH_AFTER_ACTION  1

/* set to 1 to wrap around on adjacent navigation */
#define WRAP_ADJACENT   0

#define MOD_SWITCH      MOD_ALT
#define KEY_SWITCH(n)   ('1' + (n))

#define MOD_MOVE        MOD_ALT | MOD_SHIFT
#define KEY_MOVE(n)     ('1' + (n))

#define MOD_PREV        MOD_CONTROL | MOD_ALT
#define KEY_PREV        VK_LEFT
#define MOD_NEXT        MOD_CONTROL | MOD_ALT
#define KEY_NEXT        VK_RIGHT

#define MOD_MOVE_PREV   MOD_CONTROL | MOD_ALT | MOD_SHIFT
#define KEY_MOVE_PREV   VK_LEFT
#define MOD_MOVE_NEXT   MOD_CONTROL | MOD_ALT | MOD_SHIFT
#define KEY_MOVE_NEXT   VK_RIGHT

#define MOD_LAST        MOD_CONTROL | MOD_ALT
#define KEY_LAST        VK_SPACE

#define MOD_EXIT        MOD_CONTROL | MOD_ALT
#define KEY_EXIT        'Q'

#define MOD_NEW         MOD_CONTROL | MOD_ALT
#define KEY_NEW         'N'

#define MOD_REMOVE      MOD_CONTROL | MOD_ALT
#define KEY_REMOVE      'W'
