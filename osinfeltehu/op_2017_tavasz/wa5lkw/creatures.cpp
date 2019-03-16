#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Creature
{
public:
  Creature(const string& name, int water)
    : name(name), water(water), position(0)
  {
  }

  virtual void drink(char d) = 0;
  virtual void step(char d) = 0;

  bool isAlive() const
  {
    return water > 0;
  }

  int distance() const
  {
    return position;
  }

  string getName() const
  {
    return name;
  }

  void act(char d)
  {
    if (isAlive())
    {
      drink(d);
      if (isAlive())
        step(d);
    }
  }

protected:
  string name;
  int water;
  int position;
};

class SandWalker : public Creature
{
public:
  SandWalker(const string& name, int water) : Creature(name, water)
  {
  }

  void drink(char d)
  {
    switch (d)
    {
      case 's': water -= 1; break;
      case 'c': water += 0; break;
      case 'r': water += 3; break;
    }

    water = min(8, water);
  }

  void step(char d)
  {
    switch (d)
    {
      case 's': position += 3; break;
      case 'c': position += 1; break;
      case 'r': position += 0; break;
    }
  }
};

class Sponge : public Creature
{
public:
  Sponge(const string& name, int water) : Creature(name, water)
  {
  }

  void drink(char d)
  {
    switch (d)
    {
      case 's': water -= 4; break;
      case 'c': water -= 1; break;
      case 'r': water += 6; break;
    }

    water = min(20, water);
  }

  void step(char d)
  {
    switch (d)
    {
      case 's': position += 0; break;
      case 'c': position += 1; break;
      case 'r': position += 3; break;
    }
  }
};

class Wanderer : public Creature
{
public:
  Wanderer(const string& name, int water) : Creature(name, water)
  {
  }

  void drink(char d)
  {
    switch (d)
    {
      case 's': water -= 2; break;
      case 'c': water -= 1; break;
      case 'r': water += 3; break;
    }

    water = min(8, water);
  }

  void step(char d)
  {
    switch (d)
    {
      case 's': position += 1; break;
      case 'c': position += 2; break;
      case 'r': position += 3; break;
    }
  }
};

pair<vector<Creature*>, string> readCompetition(const string& filename)
{
  vector<Creature*> creatures;

  ifstream file(filename.c_str());

  int num;
  file >> num;

  for (int i = 0; i < num; ++i)
  {
    string name;
    string type;
    int water;

    file >> name >> type >> water;

    Creature* creature;

    if (type == "sandwalker")
      creature = new SandWalker(name, water);
    else if (type == "wanderer")
      creature = new Wanderer(name, water);
    else if (type == "sponge")
      creature = new Sponge(name, water);

    creatures.push_back(creature);
  }

  string days;
  file >> days;

  file.close();

  return make_pair(creatures, days);
}

Creature* getWinner(const vector<Creature*>& creatures)
{
  Creature* winner = 0;

  for (size_t i = 0; i < creatures.size(); ++i)
    if (!winner ||
        creatures[i]->distance() > winner->distance() && winner->isAlive())
      winner = creatures[i];

  return winner;
}

int main()
{
  pair<vector<Creature*>, string> competition
    = readCompetition("creatures.txt");

  vector<Creature*>& creatures = competition.first;
  string& days = competition.second;

  Creature* winner = getWinner(creatures);

  if (!winner)
    cout << "No creature survived the competition" << endl;
  else
    cout << "The winner is: " << winner->getName() << endl;

  for (size_t i = 0; i < creatures.size(); ++i)
    delete creatures[i];

  return 0;
}
