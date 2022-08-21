enum tokens
{
   NUMBER,
   PLUS,
   MINUS,
   MULTIPLY,
   DIVIDE,
   LPAREN,
   RPAREN
};

typedef struct token
{
   enum tokens type;
   int value;
} token;

typedef struct Lexer
{
   struct token *tokens;
   int token_count;
   int currentPosition;
   char text[20];
} Lexer;

struct Node
{
   enum tokens type;
   double value;
   struct Node *left;
   struct Node *right;
} Node;

//factor function that takes the token array and the current position as parameters
struct Node *factor(struct token *tokens, int *token_count, int *currentPosition);

//term function that takes the token array and the current position as parameters
struct Node *term(struct token *tokens, int *token_count, int *currentPosition);

//expr function that takes the token array and the current position as parameters
struct Node *expr(struct token *tokens, int *token_count, int *currentPosition);