
#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include "logger.h"
using namespace std;


enum class CharacterType {
    Knight,
    Wizard,
    Archer,
    Unknown
};


class Character {
protected:
    string description;
    CharacterType type = CharacterType::Unknown;
public:
    virtual string getDescription() const { return description; }
    virtual int getAttack() const = 0;
    virtual int getSpeed() const = 0;
    virtual int getDefense() const = 0;
    virtual CharacterType getType() const { return type; }
    virtual ~Character() {
        if (this->type != CharacterType::Unknown)
            cout << "Delete Character: " << description << endl;
    }
};

// 기본 캐릭터
class Knight : public Character {
public:
    Knight() {
        description = "Knight";
        type = CharacterType::Knight;
        Logger::getInstance()->log("[Create] " + description);
    }
    int getAttack() const override { return 15; }
    int getSpeed() const override { return 8; }
    int getDefense() const override { return 20; }
};

class Wizard : public Character {
public:
    Wizard() {
        description = "Wizard";
        type = CharacterType::Wizard;
        Logger::getInstance()->log("[Create] " + description);
    }
    int getAttack() const override { return 20; }
    int getSpeed() const override { return 10; }
    int getDefense() const override { return 10; }
};

class Archer : public Character {
public:
    Archer() {
        description = "Archer";
        type = CharacterType::Archer;
        Logger::getInstance()->log("[Create] " + description);
    }
    int getAttack() const override { return 18; }
    int getSpeed() const override { return 15; }
    int getDefense() const override { return 8; }
};

// Decorator 기본 구조
class EquipDeco : public Character {
protected:
    shared_ptr<Character> character;
public:
    EquipDeco(shared_ptr<Character> c, string item) : character(c) {
        Logger::getInstance()->log(
            "[Trying to Equip] " + c->getDescription() + " + " + item
        );
    }
    virtual ~EquipDeco() { }
};

// 장비 클래스
class Armor : public EquipDeco {
public:
    Armor(shared_ptr<Character> c) : EquipDeco(c, "Armor") {} 
    string getDescription() const override { return character->getDescription() + " + Armor"; }
    int getAttack() const override { return character->getAttack(); }
    int getSpeed() const override { return character->getSpeed() - 2; }
    int getDefense() const override { return character->getDefense() + 10; }
    CharacterType getType() const override {return character->getType();}
};

class Boots : public EquipDeco {
public:
    Boots(shared_ptr<Character> c) : EquipDeco(c, "Boots") {} 
    string getDescription() const override { return character->getDescription() + " + Boots"; }
    int getAttack() const override { return character->getAttack(); }
    int getSpeed() const override { return character->getSpeed() + 5; }
    int getDefense() const override { return character->getDefense(); }
    CharacterType getType() const override {return character->getType();}
};

class Staff : public EquipDeco {
public:
    Staff(shared_ptr<Character> c) : EquipDeco(c, "Staff") {
        if (character->getType() != CharacterType::Wizard) {
            throw std::invalid_argument("Staff requires Wizard");
        }
    }
    string getDescription() const override { return character->getDescription() + " + Staff"; }
    int getAttack() const override { return character->getAttack() + 8; }
    int getSpeed() const override { return character->getSpeed(); }
    int getDefense() const override { return character->getDefense(); }
    CharacterType getType() const override {return character->getType();}
};

class Sword : public EquipDeco {
public:
    Sword(shared_ptr<Character> c) : EquipDeco(c, "Sword") {} 
    string getDescription() const override { return character->getDescription() + " + Sword"; }
    int getAttack() const override { return character->getAttack() + 10; }
    int getSpeed() const override { return character->getSpeed(); }
    int getDefense() const override { return character->getDefense(); }
    CharacterType getType() const override {return character->getType();}
};

class Bow : public EquipDeco {
public:
    Bow(shared_ptr<Character> c) : EquipDeco(c, "Bow") {
        auto t = character->getType();
        if (t != CharacterType::Archer && t != CharacterType::Knight) {
            throw std::invalid_argument("Bow requires Archer or Knight");
        }
    }
    string getDescription() const override { return character->getDescription() + " + Bow"; }
    int getAttack() const override { return character->getAttack() + 7; }
    int getSpeed() const override { return character->getSpeed() + 2; }
    int getDefense() const override { return character->getDefense(); }
    CharacterType getType() const override {return character->getType();}
};
