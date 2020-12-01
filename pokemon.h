#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QString>

using namespace std;

const double DAMAGE_COEFF = 0.65;
const double RAGE_COEFF = 1.3;
const double EXP_COEFF = 0.3;
const double COUNTER_COEFF = 1.2;
const double DECOUNTER_COEFF = 0.8;
const int EXP_TABLE[14] = { 50, 60, 70, 90, 110, 140, 170, 210, 250, 290, 340, 390, 460, 500 };

enum attribute { ATTACK, TANK, DEFENCE, SPEED };							//属性
enum kind { BASIC, FIRE, AQUA, GRASS, LIGHTNING, ROCK, MARTIAL, PSYCIC };   //系

class Pokemon
{
private:
    attribute _attri;
    kind _type;
    QString _name;
    int _level;
    int _attack;
    int _defense;
    int _max_life;
    int _current_life;
    int _speed;
    int _exp;
    int _number;

public:
    Pokemon();
    Pokemon(attribute, QString);

    virtual void Attack(Pokemon&) /* = 0*/;
    void ShowRemainingLife();
    int CalcExp(Pokemon&);
    void LevelUp();
    void LoseBattle();
    virtual double GetCounterRelation(Pokemon&);
    int CalcDamage(Pokemon&);

    //Getters & Setters
    attribute GetAttribute() { return this->_attri; }
    kind GetType() { return this->_type; }
    void SetType(kind type) { this->_type = type; }
    int GetAttack() { return this->_attack; }
    int GetDefence() { return this->_defense; }
    int GetMaxlife() { return this->_max_life; }
    int GetSpeed() { return this->_speed; }
    void SetCurrentLife(int value) { this->_current_life = value; }
    int GetCurrentLife() { return this->_current_life; }
    void SetCurrentExp(int value) { this->_exp = value; }
    int GetCurrentExp() { return this->_exp; }
    int GetCurrentLevel() { return this->_level; }
    int GetNumber() { return  this->_number; }
    QString& GetName() { return this->_name; }

    void ShowInfo();

    virtual ~Pokemon();

};

class FirePokemon : public Pokemon
{
public:
    FirePokemon();
    FirePokemon(attribute, QString);

    double GetCounterRelation(Pokemon&);

    virtual ~FirePokemon();
};

class AquaPokemon : public Pokemon
{
public:
    AquaPokemon();
    AquaPokemon(attribute, QString);

    double GetCounterRelation(Pokemon&);

    virtual ~AquaPokemon();
};

class GrassPokemon : public Pokemon
{
public:
    GrassPokemon();
    GrassPokemon(attribute, QString);

    double GetCounterRelation(Pokemon&);

    virtual ~GrassPokemon();
};

#endif // POKEMON_H
