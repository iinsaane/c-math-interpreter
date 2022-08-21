#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// include the main.h
#include "main.h"

bool is_number(char c)
{
   return c >= '0' && c <= '9';
}


   // create the lexer
   struct Lexer *lexer = malloc(sizeof(struct Lexer));
   lexer->tokens = malloc(sizeof(struct token) * 20);
   lexer->token_count = 0;
   lexer->currentPosition = 0;
   lexer->text[0] = '\0';
   char *text = lexer->text;

int main()
{
   // printf("START");

   // read the input
   printf("Enter an expression: ");
   //scanf("%s", lexer->text);
   sprintf(lexer, "1+1");
   // printf("%s\n", lexer->text);

   /**
    * LEXER
    * - tokenize the input
    */
   int i = 0;
   while (lexer->text[i] != '\0')
   {
      if (lexer->text[i] == '+')
      {
         lexer->tokens[lexer->token_count].type = PLUS;
         lexer->tokens[lexer->token_count].value = '+';
         lexer->token_count++;
      }
      else if (lexer->text[i] == '-')
      {
         lexer->tokens[lexer->token_count].type = MINUS;
         lexer->tokens[lexer->token_count].value = '-';
         lexer->token_count++;
      }
      else if (lexer->text[i] == '*')
      {
         lexer->tokens[lexer->token_count].type = MULTIPLY;
         lexer->tokens[lexer->token_count].value = '*';
         lexer->token_count++;
      }
      else if (lexer->text[i] == '/')
      {
         lexer->tokens[lexer->token_count].type = DIVIDE;
         lexer->tokens[lexer->token_count].value = '/';
         lexer->token_count++;
      }
      else if (lexer->text[i] == '(')
      {
         lexer->tokens[lexer->token_count].type = LPAREN;
         lexer->tokens[lexer->token_count].value = '(';
         lexer->token_count++;
      }
      else if (lexer->text[i] == ')')
      {
         lexer->tokens[lexer->token_count].type = RPAREN;
         lexer->tokens[lexer->token_count].value = ')';
         lexer->token_count++;
      }
      else
      {
         int number = 0;
         while (is_number(lexer->text[i]))
         {
            number = number * 10 + (lexer->text[i] - '0');
            i++;
         }
         lexer->tokens[lexer->token_count].type = NUMBER;
         lexer->tokens[lexer->token_count].value = number;
         lexer->token_count++;
         i--;
      }
      i++;
   }
   lexer->tokens[lexer->token_count].type = NUMBER;
   lexer->tokens[lexer->token_count].value = '\0';
   // lexer->token_count++;
   lexer->currentPosition = 0;
   // printf("%d\n", lexer->token_count);
   // for (i = 0; i < lexer->token_count; i++) {
   //     printf("Token %d; value %d\n", lexer->tokens[i].type, lexer->tokens[i].value);
   // }

   /**
    * PARSER
    * - parse the tokens
    */
   // parser in js
   /*
   expr() {
    var result = this.term();

    while (
      this.currentToken !== null &&
      (this.currentToken.type === tokens.PLUS ||
        this.currentToken.type === tokens.MINUS)
    ) {
      if (this.currentToken.type === tokens.PLUS) {
        this.advance();
        result = new AddNode(result, this.term());
      }
      else if (this.currentToken.type === tokens.MINUS) {
        this.advance();
        result = new SubtractNode(result, this.term());
      }
    }
    return result;
  }

  term() {
    var result = this.factor();

    while (
      this.currentToken !== null &&
      (this.currentToken.type === tokens.MULTIPLY ||
        this.currentToken.type === tokens.DIVIDE)
    ) {
      if (this.currentToken.type === tokens.MULTIPLY) {
        this.advance();
        result = new MultiplyNode(result, this.factor());
      }
      else if (this.currentToken.type === tokens.DIVIDE) {
        this.advance();
        result = new DivideNode(result, this.factor());
      }
    }
    return result;
  }

  factor() {

    if (this.currentToken.type == tokens.LPAREN) {
      this.advance();
      var result = this.expr();
      if (this.currentToken.type !== tokens.RPAREN) {
        throw new Error(`invalid syntax`);
      }
      this.advance();
      return result;
    }

    if (this.currentToken.type === tokens.NUMBER) {
      var value = this.currentToken.value;
      this.advance();
      return new NumberNode(value);
    } else if(this.currentToken.type === tokens.PLUS){
      this.advance();
      return PlusNode(this.factor());
    } else if(this.currentToken.type === tokens.MINUS){
      this.advance();
      return new SubtractNode(new NumberNode(0), this.factor());
    } else {
      throw new Error(`invalid syntax`);
    }

  }
  */
   struct token *tokens = lexer->tokens;
   int *token_count = &lexer->token_count;
   int currentPosition = lexer->currentPosition = 0;
   // make currentPosition a pointer to the lexer->tokens[lexer->currentPosition]
   struct token *currentToken = &tokens[currentPosition];
   // make a node tree
   struct Node *root = NULL;
   root = expr(tokens, token_count, &currentPosition);

   return 0;
}

// factor function that takes the token array and the current position as parameters
struct Node *factor(struct token *tokens, int *token_count, int *currentPosition)
{
   struct token *currentToken = &tokens[*currentPosition];

   struct Node *result = malloc(sizeof(struct Node));
   if (currentToken->type == LPAREN)
   {
      *currentPosition++;
      result = expr(tokens, token_count, currentPosition);
      if (currentToken->type != RPAREN)
      {
         printf("invalid syntax\n");
         exit(1);
      }
      *currentPosition++;
   }
   else if (currentToken->type == NUMBER)
   {
      result->type = NUMBER;
      result->value = currentToken->value;
      *currentPosition++;
   }
   else if (currentToken->type == PLUS)
   {
      *currentPosition++;
      result = factor(tokens, token_count, currentPosition);
   }
   else if (currentToken->type == MINUS)
   {
      *currentPosition++;
      result = factor(tokens, token_count, currentPosition);
      result->value = -result->value;
   }
   else
   {
      printf("invalid syntax\n");
      exit(1);
   }
   return result;
}

// term function that takes the token array and the current position as parameters
struct Node *term(struct token *tokens, int *token_count, int *currentPosition)
{
   struct token *currentToken = &tokens[*currentPosition];

   struct Node *result = malloc(sizeof(struct Node));
   result->left = factor(tokens, token_count, currentPosition);
   while (currentToken->type == MULTIPLY || currentToken->type == DIVIDE)
   {
      if (currentToken->type == MULTIPLY)
      {
         *currentPosition++;
         result->type = MULTIPLY;
         result->right = factor(tokens, token_count, currentPosition);
      }
      else if (currentToken->type == DIVIDE)
      {
         *currentPosition++;
         result->type = DIVIDE;
         result->right = factor(tokens, token_count, currentPosition);
      }
   }
   return result;
}

// expr function that takes the token array and the current position as parameters
struct Node *expr(struct token *tokens, int *token_count, int *currentPosition)
{
   struct token *currentToken = &tokens[*currentPosition];

   struct Node *result = malloc(sizeof(struct Node));
   result->left = term(tokens, token_count, currentPosition);

   while (currentToken->type == PLUS || currentToken->type == MINUS)
   {
      if (currentToken->type == PLUS)
      {
         *currentPosition++;
         result->type = PLUS;
         result->right = term(tokens, token_count, currentPosition);
      }
      else if (currentToken->type == MINUS)
      {
         *currentPosition++;
         result->type = MINUS;
         result->right = term(tokens, token_count, currentPosition);
      }
   }
   return result;
}
