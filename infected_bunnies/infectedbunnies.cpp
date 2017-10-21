// ConsoleApplication28.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>

//***************************************************************************
//							CLASS DECLARATION
//***************************************************************************


class Bunny
{
public:
	Bunny() : age(1) {
		choose_sex();
		choose_color();
		choose_name();
		is_radioactive();
	}
	~Bunny() {}

	char sex;
	std::string color;
	int age;
	int max_age;
	std::string name;
	bool radioactive;

	void got_radioactive();

private:

	void choose_sex();
	void choose_color();
	void choose_name();
	void is_radioactive();

};

//***************************************************************************
//							CLASS FUNCTIONS
//***************************************************************************


void Bunny::got_radioactive() {
	radioactive = true;
	max_age = 50;
}

void Bunny::choose_sex() {
	char sexes[] = { 'M', 'F' };
	int Rand = rand() % 2;
	sex = sexes[Rand];
}

void Bunny::choose_color() {
	std::string colors[] = { "white", "brown", "black", "spotted" };
	int Rand = rand() % 4;
	color = colors[Rand];
}

void Bunny::choose_name() {
	std::string names[7] = { "Thumper", "Oreo", "Daisy", "Bella", "Charlie", "Lola", "Lily" };
	int Rand = rand() % 7;
	name = names[Rand];
}

void Bunny::is_radioactive() {
	int Rand = rand() % 50;

	if (Rand == 0)
		got_radioactive();
	else {
		radioactive = false;
		max_age = 10;
	}
}



//***************************************************************************
//							FUNCTIONS
//***************************************************************************

using namespace std;

void turn_one(vector<Bunny>& bunnies) {
	for (auto i = 0; i < 5; ++i) {
		Bunny rabbit;
		bunnies.push_back(rabbit);
	}
}


bool is_male(Bunny& rabbit) {
	return (rabbit.sex == 'M' && rabbit.age > 2 && !rabbit.radioactive);
}


bool male_ready(vector<Bunny>& bunnies) {

	bool is_ready;
	if (find_if(bunnies.begin(), bunnies.end(), is_male) != bunnies.end())
		is_ready = true;
	else
		is_ready = false;

	return is_ready;
}


void out_records(vector<Bunny>& bunnies) {

	if (!bunnies.empty()) {
		for (auto rabbit : bunnies) {
			cout << "Name: " << rabbit.name << endl
				<< "Sex: " << rabbit.sex << endl
				<< "Color: " << rabbit.color << endl
				<< "Age: " << rabbit.age << endl
				<< "Radioactive: " << std::boolalpha << rabbit.radioactive << std::noboolalpha << endl << endl;
		}
	}

}

void infection(vector<Bunny>& bunnies, int how_many) {

	int size = bunnies.end() - bunnies.begin();

	int i = 0;

	while (i < how_many) {

		int Rand = rand() % size;
		auto bunny = bunnies.begin() + Rand;

		if (bunny->radioactive == false) {
			++i;
			bunny->Bunny::got_radioactive();

		}
	}
}

void how_many_bunnies(vector<Bunny>& bunnies, int to_infect) {

	cout << "This round " << to_infect << " out of " << (bunnies.end() - bunnies.begin()) << " bunnies will be infected." << endl;

	if (to_infect > ((bunnies.end() - bunnies.begin()) / 2)) {
		cout << "INFECTED BUNNIES HAVE WON!!!!" << endl;
		exit(0);
	}

}


vector<Bunny>::iterator aging(vector<Bunny>& bunnies, vector<Bunny>::iterator bunny) {
	vector<Bunny>::iterator new_bunny;
	bunny->age += 1;

	if (bunny->age > bunny->max_age) {
		cout << "Bunny has died!!" << endl;
		new_bunny = bunnies.erase(bunny);
	}
	else
		new_bunny = bunny + 1;

	return new_bunny;
}


//contains reproduction, infection and dying
vector<Bunny> separation(vector<Bunny>& bunnies) {

	vector<Bunny> to_insert;
	bool male = false;
	int to_infect = 0;

	male = male_ready(bunnies);

	auto bunny = bunnies.begin();

	while (bunny != bunnies.end()) {
		bool radio = bunny->radioactive;

		if (male == true) {
			if (bunny->sex == 'F' && !radio) {
				Bunny new_one;
				new_one.color = bunny->color;
				to_insert.push_back(new_one);
			}
		}

		if (radio)
			to_infect += 1;

		bunny = aging(bunnies, bunny);

	}

	how_many_bunnies(bunnies, to_infect);

	infection(bunnies, to_infect);

	return to_insert;
}


int main()
{
	srand((int)time(0));
	vector<Bunny> bunnies;
	turn_one(bunnies);
	bool male = false;

	for (auto i = 1; i < 50; ++i) {

		vector<Bunny> to_insert;

		if (!bunnies.empty())
			to_insert = separation(bunnies);

		bunnies.insert(bunnies.end(), to_insert.begin(), to_insert.end());

		cout << "Round " << i << " finished" << endl;

		
		this_thread::sleep_for(chrono::milliseconds(1000));
	}

	out_records(bunnies);

	return 0;
}
