#include "pokemon.h"

Pokemon::Pokemon()
{
    cout << "调用了pokemon抽象类的默认构造函数" << endl;

    this->_exp = 0;
    this->_level = 1;
    this->_name = "squirtle";
    this->_type = BASIC;

    srand((int)time(NULL));

    this->_attri = (attribute) (rand() % 4);
    this->_number = rand();

    switch (this->_attri) {
    case ATTACK:
        this->_attack = 30;
        this->_defense = 20;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 20;
        break;

    case TANK:
        this->_attack = 20;
        this->_defense = 20;
        this->_max_life = 30;
        this->_current_life = 30;
        this->_speed = 20;
        break;

    case DEFENCE:
        this->_attack = 20;
        this->_defense = 30;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 20;
        break;

    case SPEED:
        this->_attack = 20;
        this->_defense = 20;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 30;
        break;

    default:
        this->_attack = 20;
        this->_defense = 20;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 20;
        break;

    }
}

Pokemon::Pokemon(attribute attri, QString name) : _attri(attri), _name(name)
{
    cout << "调用了pokemon抽象类的二参构造函数" << endl;

    this->_exp = 0;
    this->_level = 1;
    this->_type = BASIC;
    this->_number = rand();

    switch (this->_attri) {
    case ATTACK:
        this->_attack = 30;
        this->_defense = 20;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 20;
        break;

    case TANK:
        this->_attack = 20;
        this->_defense = 20;
        this->_max_life = 30;
        this->_current_life = 30;
        this->_speed = 20;
        break;

    case DEFENCE:
        this->_attack = 20;
        this->_defense = 30;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 20;
        break;

    case SPEED:
        this->_attack = 20;
        this->_defense = 20;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 30;
        break;

    default:
        this->_attack = 20;
        this->_defense = 20;
        this->_max_life = 20;
        this->_current_life = 20;
        this->_speed = 20;
        break;

    }
}

void Pokemon::Attack(Pokemon& subject)
{
    int dodge_flag = rand() % 100;

    cout << "dodge_flag=" << dodge_flag << endl;

    if (dodge_flag < 5) {
        //cout << subject.GetName() << "闪避了这次攻击！" << endl;
    }
    else {
        int damage = this->CalcDamage(subject);
        int rage_flag = rand() % 100;

        subject.SetCurrentLife(subject.GetCurrentLife() - damage);
        if (subject.GetCurrentLife() < 0)
            subject.SetCurrentLife(0);

        //cout << this->GetName() << "对" << subject.GetName() << "做出了攻击，"
            //<< subject.GetName() << "受到了" << damage << "点伤害！" << endl;
    }
}

void Pokemon::ShowRemainingLife()
{
    //cout << this->_name << "还剩" << this->_current_life << "点生命值！" << endl;
}

void Pokemon::LoseBattle()
{
    //cout << this->_name << "被击倒！" << endl;
}

int Pokemon::CalcExp(Pokemon&)
{
    return 500;
}

void Pokemon::LevelUp()
{
    //cout << this->_name << "从" << this->_level << "级升到了" << this->_level + 1 << "级！" << endl;
    this->_exp = this->_exp - EXP_TABLE[this->_level - 1];
    this->_level++;

    switch (this->_attri) {
    case ATTACK:
        this->_attack += 4;
        this->_defense += 3;
        this->_max_life += 3;
        this->_current_life += 3;
        this->_speed += 2;
        break;

    case TANK:
        this->_attack += 3;
        this->_defense += 2;
        this->_max_life += 4;
        this->_current_life += 4;
        this->_speed += 3;
        break;

    case DEFENCE:
        this->_attack += 2;
        this->_defense += 4;
        this->_max_life += 3;
        this->_current_life += 3;
        this->_speed += 3;
        break;

    case SPEED:
        this->_attack += 2;
        this->_defense += 3;
        this->_max_life += 3;
        this->_current_life += 3;
        this->_speed += 4;
        break;

    default:
        this->_attack += 3;
        this->_defense += 3;
        this->_max_life += 3;
        this->_current_life += 3;
        this->_speed += 3;
        break;
    }
}

void Pokemon::ShowInfo()
{
    cout << "精灵信息如下：" << endl;
    //cout << "名称：" << this->_name << endl;
    cout << "等级：" << this->_level << endl;
    cout << "经验：" << this->_exp << endl;
    cout << "攻击：" << this->_attack << endl;
    cout << "防御：" << this->_defense << endl;
    cout << "最大生命值：" << this->_max_life << endl;
    cout << "当前生命值：" << this->_current_life << endl;
    cout << "速度：" << this->_speed << endl;

    cout << "属性：";
    switch (this->_attri) {
    case ATTACK:
        cout << "攻击型" << endl; break;
    case DEFENCE:
        cout << "防御型" << endl; break;
    case TANK:
        cout << "肉盾型" << endl; break;
    case SPEED:
        cout << "速度型" << endl; break;
    default:
        cout << "无属性" << endl; break;
    }

    cout << "系：";
    switch (this->_type) {
    case FIRE:
        cout << "火系\n" << endl; break;
    case AQUA:
        cout << "水系\n" << endl; break;
    case GRASS:
        cout << "草系\n" << endl; break;
    case LIGHTNING:
        cout << "电系\n" << endl; break;
    case ROCK:
        cout << "岩石系\n" << endl; break;
    case MARTIAL:
        cout << "格斗系\n" << endl; break;
    case PSYCIC:
        cout << "超能力系\n" << endl; break;
    default:
        cout << "普通系\n" << endl; break;
    }

}

int Pokemon::CalcDamage(Pokemon& subject)
{
    double counter = this->GetCounterRelation(subject);

    //if(rage_flag == 1) {
        //return (int) (this->_attack * this->_attack / subject._defense * DAMAGE_COEFF * counter * RAGE_COEFF);
    return (int) (this->_attack * this->_attack / subject._defense * DAMAGE_COEFF * counter);
}

double Pokemon::GetCounterRelation(Pokemon&)
{
    cout << "Pokemon基类中的克制函数，在此不实现" << endl;

    return -1.0;
}

Pokemon::~Pokemon()
{
    cout << "调用了Pokemon抽象类的析构函数" << endl;
}

FirePokemon::FirePokemon() : Pokemon()
{
    this->SetType(FIRE);
}

FirePokemon::FirePokemon(attribute attri, QString name) : Pokemon(attri, name)
{
    this->SetType(FIRE);
}

double FirePokemon::GetCounterRelation(Pokemon& subject)
{
    switch (subject.GetType()) {
    case GRASS: case MARTIAL:
        return COUNTER_COEFF;

    case AQUA: case ROCK:
        return DECOUNTER_COEFF;

    default:
        return 1.0;
    }
}

FirePokemon::~FirePokemon()
{
    cout << "调用了FirePokemon类的析构函数" << endl;
}

AquaPokemon::AquaPokemon() : Pokemon()
{
    this->SetType(AQUA);
}

AquaPokemon::AquaPokemon(attribute attri, QString name) : Pokemon(attri, name)
{
    this->SetType(AQUA);
}

double AquaPokemon::GetCounterRelation(Pokemon& subject)
{
    switch (subject.GetType()) {
    case FIRE: case ROCK:
        return COUNTER_COEFF;

    case GRASS: case LIGHTNING:
        return DECOUNTER_COEFF;

    default:
        return 1.0;
    }
}

AquaPokemon::~AquaPokemon()
{
    cout << "调用了AquaPokemon类的析构函数" << endl;
}

GrassPokemon::GrassPokemon() : Pokemon()
{
    this->SetType(GRASS);
}

GrassPokemon::GrassPokemon(attribute attri, QString name) : Pokemon(attri, name)
{
    this->SetType(GRASS);
}

double GrassPokemon::GetCounterRelation(Pokemon& subject)
{
    switch (subject.GetType()) {
    case AQUA: case ROCK:
        return COUNTER_COEFF;

    case FIRE: case PSYCIC:
        return DECOUNTER_COEFF;

    default:
        return 1.0;
    }
}

GrassPokemon::~GrassPokemon()
{
    cout << "调用了GrassPokemon类的析构函数" << endl;
}
