#include "tokenizer.h"

Tokenizer::Tokenizer(std::string code)
{
    this->code = code;
}

int Tokenizer::tokenize()
{
    std::stringstream stream(code);
    std::string tok;
    while (stream >> tok)
    {
        if (m.count(tok))
            tokens.push_back(m.at(tok));
        else
        {
            token tk;
            int val = convertStrToInt(tok);
            if (val == -1)
            {
                tk.type = token_type::STRING_LITERAL;
                tk.val_str = tok;
            }
            else
            {
                tk.type = token_type::INT_LITERAL;
                tk.val_int = val;
            }
            tokens.push_back(tk);
        }
    }
    return 0;
}

std::vector<Tokenizer::token> &Tokenizer::get_tokens()
{
    return this->tokens;
}

int Tokenizer::convertStrToInt(std::string str)
{
    for (auto ch : str)
    {
        if (!isInt(ch))
            return -1;
    }
    return stoi(str);
}

inline bool Tokenizer::isInt(char ch)
{
    return (ch >= '0' && ch <= '9');
}