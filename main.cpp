#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <sstream>
#include <map>

struct Token {
    std::string text;
    std::string type;
    int line;
};

struct Symbol {
    std::string name;
    std::string type;   // e.g., "int" or "unknown"
    std::string scope;  // simplified: "global" or "main"
    int declLine;
    std::string value;  // if assigned
    bool declared;
};

bool isKeyword(const std::string& s) {
    static std::unordered_set<std::string> keywords = {
        "int", "return", "if", "else", "for", "while", "do", "break", "continue", "void", "char", "float", "double"
    };
    return keywords.count(s) > 0;
}

bool isOperatorChar(char c) {
    static std::string ops = "+-*/%=<>!&|^~";
    return ops.find(c) != std::string::npos;
}

bool isPunctuation(char c) {
    static std::string punc = "();,{}[]:.";
    return punc.find(c) != std::string::npos;
}

int main() {
    std::ifstream file("Sample.txt");
    if (!file.is_open()) {
        std::cerr << "Cannot open Sample.txt\n";
        return 1;
    }

    std::vector<Token> tokens;
    std::map<std::string, Symbol> symTable;
    std::string line;
    int lineNo = 0;
    std::string currentScope = "global";

    while (std::getline(file, line)) {
        lineNo++;
        size_t i = 0;
        while (i < line.size()) {
            char c = line[i];

            // Skip whitespace
            if (std::isspace((unsigned char)c)) { i++; continue; }

            // Identifier or keyword (start with letter or _)
            if (std::isalpha((unsigned char)c) || c == '_') {
                std::string s;
                while (i < line.size() && (std::isalnum((unsigned char)line[i]) || line[i] == '_')) {
                    s.push_back(line[i++]);
                }
                if (isKeyword(s)) {
                    tokens.push_back({s, "keyword", lineNo});
                    // handle scope detection for a simple case: functions like main()
                    if (s == "int") {
                        // we keep 'int' as a keyword; following tokens handled below
                    }
                } else {
                    tokens.push_back({s, "identifier", lineNo});
                }
                continue;
            }

            // Number literal (integer or float)
            if (std::isdigit((unsigned char)c)) {
                std::string num;
                while (i < line.size() && (std::isdigit((unsigned char)line[i]) || line[i] == '.')) {
                    num.push_back(line[i++]);
                }
                tokens.push_back({num, "number", lineNo});
                continue;
            }

            // String literal
            if (c == '\"') {
                std::string str;
                str.push_back(line[i++]); // leading "
                while (i < line.size()) {
                    str.push_back(line[i]);
                    if (line[i] == '"' && str.back() == '"') { i++; break; }
                    i++;
                }
                tokens.push_back({str, "string_literal", lineNo});
                continue;
            }

            // Multi-char operators first (<<, >>, <=, >=, ==, !=, &&, ||, ++, --)
            if (isOperatorChar(c)) {
                std::string op;
                op.push_back(c);
                if (i + 1 < line.size()) {
                    char c2 = line[i+1];
                    std::string two = op + c2;
                    static std::unordered_set<std::string> multiOps = {
                        "<<", ">>", "<=", ">=", "==", "!=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "%="
                    };
                    if (multiOps.count(two)) {
                        op = two;
                        i += 2;
                        tokens.push_back({op, "operator", lineNo});
                        continue;
                    }
                }
                // single-char operator
                i++;
                tokens.push_back({op, "operator", lineNo});
                continue;
            }

            // Punctuation
            if (isPunctuation(c)) {
                std::string p(1, c);
                tokens.push_back({p, "punctuation", lineNo});
                // detect scope changes
                if (c == '{') currentScope = "main"; // simplistic: assume code in main
                if (c == '}') currentScope = "global";
                i++;
                continue;
            }

            // Anything else (treat as single char token)
            std::string other(1, c);
            tokens.push_back({other, "unknown", lineNo});
            i++;
        } // end char loop
    } // end lines

    // Simple pass to build symbol table from tokens:
    // - When we see 'int' followed by identifiers (possibly comma-separated) and ending with ';', register them.
    // - When we see identifier followed by '=' assign value (look ahead).
    // - Mark used identifiers that were not declared.

    for (size_t idx = 0; idx < tokens.size(); ++idx) {
        Token tk = tokens[idx];

        // declaration: keyword 'int'
        if (tk.type == "keyword" && tk.text == "int") {
            // collect following identifiers until ';'
            size_t j = idx + 1;
            while (j < tokens.size()) {
                if (tokens[j].type == "identifier") {
                    std::string name = tokens[j].text;
                    if (symTable.find(name) == symTable.end()) {
                        symTable[name] = {name, "int", "main", tokens[j].line, "", true};
                    } else {
                        symTable[name].declared = true;
                        symTable[name].type = "int";
                        symTable[name].declLine = tokens[j].line;
                    }
                    j++;
                } else if (tokens[j].text == ",") {
                    j++;
                } else if (tokens[j].text == ";") {
                    break;
                } else {
                    j++;
                }
            }
        }

        // assignment: identifier '=' number or identifier '=' string
        if (tk.type == "identifier" && (idx + 1) < tokens.size() && tokens[idx+1].text == "=") {
            std::string name = tk.text;
            std::string val = "";
            if (idx + 2 < tokens.size()) val = tokens[idx+2].text;
            // create symbol entry if not present
            if (symTable.find(name) == symTable.end()) {
                symTable[name] = {name, "unknown", "main", -1, val, false};
            } else {
                symTable[name].value = val;
            }
        }

        // usage: any identifier -> if not declared, add with declared=false
        if (tk.type == "identifier") {
            std::string name = tk.text;
            if (symTable.find(name) == symTable.end()) {
                symTable[name] = {name, "unknown", "main", -1, "", false};
            }
        }
    }

    // Print tokens (grouped by line)
    std::cout << "---------------------- Tokens ----------------------\n";
    std::cout << "Text\t\tType\t\tLine\n";
    std::cout << "----------------------------------------------------\n";
    for (const auto &t : tokens) {
        std::cout << t.text << "\t\t" << t.type << "\t\t" << t.line << "\n";
    }

    // Print symbol table
    std::cout << "\n-------------------- Symbol Table ------------------\n";
    std::cout << "Name\tType\tDeclared\tScope\tDeclLine\tValue\n";
    std::cout << "----------------------------------------------------\n";
    for (const auto &p : symTable) {
        const Symbol &s = p.second;
        std::cout << s.name << "\t"
                  << s.type << "\t"
                  << (s.declared ? "yes" : "no") << "\t\t"
                  << s.scope << "\t"
                  << (s.declLine >= 0 ? std::to_string(s.declLine) : "-") << "\t\t"
                  << s.value << "\n";
    }

    file.close();
    return 0;
}
