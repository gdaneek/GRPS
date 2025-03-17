#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <memory>
#include <unordered_map> 

class StateManager;

class State {
public:
    virtual ~State() = default;
    virtual void process(char c, std::vector<std::string>& tokens, std::string& buffer) = 0;
};

class StateManager {
public:
    enum class StateType { 
        ExpectNewToken, 
        ReadingIdentifier, 
        ReadingNumber 
    };

    static void initialize();
    static void setState(StateType type);
    static void process(char c, std::vector<std::string>& tokens, std::string& buffer);

private:
    static std::shared_ptr<State> currentState;
    static std::unordered_map<StateType, std::shared_ptr<State>> states;
};

class ExpectNewTokenState : public State {
public:
    void process(char c, std::vector<std::string>& tokens, std::string& buffer) override {
        if (isspace(c)) return;

        if (c == '*' || c == '=' || c == '&' || c == ';') {
            tokens.push_back(std::string(1, c));
        } else if (isalpha(c) || c == '_') {
            buffer += c;
            StateManager::setState(StateManager::StateType::ReadingIdentifier);
        } else if (isdigit(c)) {
            buffer += c;
            StateManager::setState(StateManager::StateType::ReadingNumber);
        }
    }
};

class ReadingIdentifierState : public State {
public:
    void process(char c, std::vector<std::string>& tokens, std::string& buffer) override {
        if (isalnum(c) || c == '_') {
            buffer += c;
        } else {
            tokens.push_back(buffer);
            buffer.clear();
            StateManager::setState(StateManager::StateType::ExpectNewToken);
            StateManager::process(c, tokens, buffer);
        }
    }
};

class ReadingNumberState : public State {
public:
    void process(char c, std::vector<std::string>& tokens, std::string& buffer) override {
        if (isdigit(c)) {
            buffer += c;
        } else {
            tokens.push_back(buffer);
            buffer.clear();
            StateManager::setState(StateManager::StateType::ExpectNewToken);
            StateManager::process(c, tokens, buffer);
        }
    }
};

std::shared_ptr<State> StateManager::currentState;
std::unordered_map<StateManager::StateType, std::shared_ptr<State>> StateManager::states;

void StateManager::initialize() {
    states = {
        {StateType::ExpectNewToken, std::make_shared<ExpectNewTokenState>()},
        {StateType::ReadingIdentifier, std::make_shared<ReadingIdentifierState>()},
        {StateType::ReadingNumber, std::make_shared<ReadingNumberState>()}
    };
    currentState = states[StateType::ExpectNewToken];
}

void StateManager::setState(StateType type) {
    currentState = states[type];
}

void StateManager::process(char c, std::vector<std::string>& tokens, std::string& buffer) {
    currentState->process(c, tokens, buffer);
}

class FileToTokensConverter {
public:
    void f(std::ifstream& in, std::vector<std::string>& tokens) {
        StateManager::initialize();
        std::string buffer;
        char c;

        while (in.get(c)) {
            StateManager::process(c, tokens, buffer);
        }

        if (!buffer.empty()) {
            tokens.push_back(buffer);
        }
    }
};

int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "File not found!" << std::endl;
        return 1;
    }

    FileToTokensConverter converter;
    std::vector<std::string> tokens;
    converter.f(file, tokens);

    std::cout << "Токены: [";
    for (const auto& token : tokens) {
        std::cout << "\"" << token << "\", ";
    }
    std::cout << "]" << std::endl;

    return 0;
}
