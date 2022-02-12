#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

//#define _SIMPLE_TIMER_H // для розрахунку часу, яка потрібна програмі, щоб виконати щось
#ifdef _SIMPLE_TIMER_H
#include "SimpleTimer.h"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <Windows.h>
#include <typeinfo>
#include <memory>
#include <iomanip>
#include <ctime>


typedef int Date[3];

using namespace std;


//перевірка чи дата a є меншою або рівною за дату b
bool smallerOrEqual(int* a, int* b)
{
	if (*a == *b && *(a + 1) == *(b + 1) && *(a + 2) == *(b + 2)) return 1;//якщо дати співпадають
	if (*(a + 2) < *(b + 2)) return 1; // порівнюємо роки
	if (*(a + 2) == *(b + 2) && *(a + 1) < *(b + 1)) return 1; // порівнюємо місяці
	if (*(a + 2) == *(b + 2) && *(a + 1) == *(b + 1) && *a < *b) return 1; // порівнюємо дні
	return 0;
}
bool smallerOrEqual(vector<int> a, vector<int> b)
{
	if (a[0] == b[0] && a[1] == b[1] && a[2] == b[2]) return 1;
	if (a[2] < b[2]) return 1;
	if (a[2] == b[2] && a[1] < b[1]) return 1;
	if (a[2] == b[2] && a[1] == b[1] && a[0] < b[0]) return 1;
	return 0;
}
const int monthDays[12]
= { 31, 28, 31, 30, 31, 30,
   31, 31, 30, 31, 30, 31 };
int countLeapYears(vector<int> d)//високосні роки
{
	int years = d[2];
	if (d[1] <= 2)
		years--;
	return years / 4
		- years / 100
		+ years / 400;
}
int countLeapYears(Date d)//високосні роки
{
	int years = d[2];
	if (d[1] <= 2)
		years--;
	return years / 4
		- years / 100
		+ years / 400;
}

int getDifference(vector<int> dt1, vector<int> dt2)
{

	long int n1 = dt1[2] * 365 + dt1[0];

	for (int i = 0; i < dt1[1] - 1; i++)
		n1 += monthDays[i];


	n1 += countLeapYears(dt1);


	long int n2 = dt2[2] * 365 + dt2[0];
	for (int i = 0; i < dt2[1] - 1; i++)
		n2 += monthDays[i];
	n2 += countLeapYears(dt2);

	return (n2 - n1);
}
int getDifference(Date dt1, Date dt2)
{
	long int n1 = dt1[2] * 365 + dt1[0];
	for (int i = 0; i < dt1[1] - 1; i++)
		n1 += monthDays[i];
	n1 += countLeapYears(dt1);
	long int n2 = dt2[2] * 365 + dt2[0];
	for (int i = 0; i < dt2[1] - 1; i++)
		n2 += monthDays[i];
	n2 += countLeapYears(dt2);
	return (n2 - n1);
}

bool DateBetweenOrEqual(int* first, int* ToCompare, int* last)
{
	return (smallerOrEqual(first, ToCompare) && smallerOrEqual(ToCompare, last));
}

bool DateBetweenOrEqual(vector<int> first, vector<int> ToCompare, vector<int> last)
{
	return (smallerOrEqual(first, ToCompare) && smallerOrEqual(ToCompare, last));
}
void DelimString(string delim, string str, vector<string>& tempVector)// поділ рядка на підрядки та запис підрядків до вектора
{
	size_t prev = 0;
	size_t next;
	size_t delta = delim.length();
	while ((next = str.find(delim, prev)) != string::npos)
	{
		tempVector.push_back(str.substr(prev, next - prev));
		prev = next + delta;
	}
	tempVector.push_back(str.substr(prev));
}

void ConvertToDate(string delim, string str, int* date)
{
	vector<string> tempVector;
	DelimString(delim, str, tempVector);
	*date = stoi(tempVector[0]);
	*(date + 1) = stoi(tempVector[1]);
	*(date + 2) = stoi(tempVector[2]);
}
///////////////				ROOM			/////////////////

class additionalService
{
public:
	additionalService()
	{
		name = "";
		price = -1;// не має такої функції
		isIncluded = false;
	}
	string name;
	double price;
	bool isIncluded; // true - входить в ціну кімнати, false - ni

};

class additionalServiceUsed : public additionalService
{
public:
	size_t corpsnumber;
	Date date;
};

class Booking;
class Booking_client;
class Booking_company;

class Client
{
public:
	Client() : name(""), surname("") {};
	string name;
	string surname;
	vector<Booking_client*> _bookingClient;
	vector<string> _complaint;
	vector<additionalServiceUsed> _additionalService;
};

class Company
{
public:
	Company() = default;
	string name;
	Date contractFrom;
	Date contractTo;
	vector<Booking_company*> _bookingCompany;
};

class Booking
{
public:

	Booking() {};
	bool IsCanceled;
	Date BookedFrom;
	Date BookedTo;
	size_t roomNumber;
	size_t corpsNumber;
};

class Booking_client : public Booking
{
public:
	Client client;
};

class Booking_company : public Booking
{
public:
	Company company;
};
class spendingOnRoom //витрати на кімнату
{
public:
	Date dateOfPayment;
	size_t costs;
};

class room_in_hotel  // для клієнтів
{
public:
	size_t roomNumber;
	size_t capacity;// місцевість   
	size_t pricePerDay;
	room_in_hotel();
	room_in_hotel(size_t roomNumber, size_t capacity, size_t pricePerDay);
	vector<Booking_client*> _bookingHistoryClients;
	vector<Booking_company*> _bookingHistoryCompanies;
	vector<spendingOnRoom*> _spendingOnRoom;
};
room_in_hotel::room_in_hotel()
{
	roomNumber = capacity = pricePerDay = 0;
}

room_in_hotel::room_in_hotel(size_t roomNumber, size_t capacity, size_t pricePerDay) :
	roomNumber(roomNumber), capacity(capacity), pricePerDay(pricePerDay) {}


////////////////////////////					STOREY				///

class storey
{
public:
	storey() { storeyNumber = 0; }
	storey(size_t corpsNumber, size_t storeyNumber, vector<room_in_hotel*> _rooms) :
		storeyNumber(storeyNumber), _rooms(_rooms) {}
	size_t storeyNumber;
	vector<room_in_hotel*> _rooms;
private:


};

/////////////												CORPS							//
class corps
{
public:
	corps();
	corps(size_t star_rating, size_t corps_number, vector<storey*>& _storeys);
	size_t corps_number;
	size_t star_rating;
	vector<storey*> _storeys;
	vector<additionalService> _additionalServices;
};

corps::corps()
{
	star_rating = corps_number = 0;
	_storeys = {};
	_additionalServices = {};

}

corps::corps(size_t star_rating, size_t corps_number, vector<storey*>& _storeys)
{
	this->star_rating = star_rating;
	this->_storeys = _storeys;
	this->corps_number = corps_number;
}

////////////////		HOTEL_COMPLEX						//

class hotel_complex
{
public:
	vector<corps*> _corps;
	hotel_complex()
	{
		_corps = {};
	}
	hotel_complex(vector<corps*> _corps) : _corps(_corps) {}
	~hotel_complex() = default;
	vector<Company*> _companies;
	vector<Client*> _Clients;

};

// Глобальні змінні			//

hotel_complex* HC = new hotel_complex;
int discount = 50;// знижка для організацій( 50 == 50%)
bool ErrorAppeared = false;

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CorpsExist(hotel_complex* HC, int corpsNumber)
{
	bool Exist = false;
	for (auto corpsEl : HC->_corps)
		if (corpsEl->corps_number == corpsNumber) Exist = true;
	return Exist;
}
bool RoomExist(hotel_complex* HC, int corpsNumber, int RoomNumber)//перевірка чи кімната з певним номером існує
{
	bool Exist = false;
	for (auto corpsEl : HC->_corps)
	{
		if (corpsEl->corps_number == corpsNumber)
		{
			for (auto storeyEl : corpsEl->_storeys)
			{
				for (auto roomEl : storeyEl->_rooms)
				{
					if (roomEl->roomNumber == RoomNumber) { Exist = true; goto tobreak; }
				}
			}
		}
	}
tobreak:
	return Exist;
}
// для додавання даних в вектор по індексу, навіть якщо величина вектора менша ніж та, яка потрібна
template<typename T>
void addToVector(std::vector<T*>& vc, int positionOfData, T* data)
{
	if (positionOfData > vc.size())
		vc.resize(positionOfData);
	vc[positionOfData - 1] = data;
}

template<typename T>
void addToDateVector(std::vector<vector<T>>& vc, int positionOfData, T* date)//масив
{
	if (positionOfData > vc.size())
		vc.resize(positionOfData);
	vc[positionOfData - 1] = data;
}
// ініціалізація корпусів за допомогою файла
void addingNewRoom(const vector<string>& tempVector, hotel_complex* HC, corps* currentCorps, storey* currentStorey, room_in_hotel* currentRoom)// нова кімната

{
	currentRoom->roomNumber = stoi(tempVector[3]);
	currentRoom->capacity = stoi(tempVector[4]);
	currentRoom->pricePerDay = stoi(tempVector[5]);
	currentStorey->_rooms.push_back(currentRoom);

}

void addingNewStorey(const vector<string>& tempVector, hotel_complex* HC, corps* currentCorps, storey* currentStorey, room_in_hotel* currentRoom) // новий поверх

{
	addingNewRoom(tempVector, HC, currentCorps, currentStorey, currentRoom);
	currentStorey->storeyNumber = stoi(tempVector[2]);
	currentCorps->_storeys.push_back(currentStorey);

}

void addingNewCorps(const vector<string>& tempVector, hotel_complex* HC, corps* currentCorps, storey* currentStorey, room_in_hotel* currentRoom) // новий корпус

{
	addingNewStorey(tempVector, HC, currentCorps, currentStorey, currentRoom);
	currentCorps->corps_number = stoi(tempVector[0]);
	currentCorps->star_rating = stoi(tempVector[1]);
	HC->_corps.push_back(currentCorps);
}

void InitComplexFromFile(hotel_complex* HC, string pathToTheFile) //переписування даних з файла в екземпляр класа 
{
	ifstream outData;
	string DataFromFile;
	vector<string> tempVector; // для даних з файлу 
	outData.open(pathToTheFile);
	if (outData.is_open())
	{
		std::getline(outData, DataFromFile);
		DelimString("	", DataFromFile, tempVector);

		corps* currentCorps = new corps; // перший по номеру корпус з файлу
		storey* currentStorey = new storey;// перший поверх з файлу
		room_in_hotel* currentRoom = new room_in_hotel; // перша кімната з файлу

		addingNewCorps(tempVector, HC, currentCorps, currentStorey, currentRoom);
		tempVector.clear();

		while (!outData.eof())
		{
			std::getline(outData, DataFromFile);
			DelimString("	", DataFromFile, tempVector);
			if (tempVector.size() < 6)
				break;



			if (stoi(tempVector[0]) != currentCorps->corps_number)// новий корпус
			{
				currentCorps = new corps;
				currentStorey = new storey;
				currentRoom = new room_in_hotel;
				addingNewCorps(tempVector, HC, currentCorps, currentStorey, currentRoom);
			}
			else
			{
				if (stoi(tempVector[2]) != currentStorey->storeyNumber)//новий поверх
				{
					currentStorey = new storey;
					currentRoom = new room_in_hotel;
					addingNewStorey(tempVector, HC, currentCorps, currentStorey, currentRoom);
				}
				else
				{
					currentRoom = new room_in_hotel;
					addingNewRoom(tempVector, HC, currentCorps, currentStorey, currentRoom);// нова кімната
				}
			}

			tempVector.clear();
		}
		//запис даних в 
	}
	else
	{
		std::cout << "Помилка пiд час вiдкривання файлу з iнформацiєю про корпуси!" << endl;
		ErrorAppeared = true;
	}
}

// очистка пам'яті  /////
void DeleteHotelComplex(hotel_complex* HC)
{
	if (HC != nullptr)
	{
		for (auto& corpsDel : HC->_corps)// корпуси
		{
			if (corpsDel != nullptr)
			{
				for (auto& storeyDel : corpsDel->_storeys)//поверхи
				{
					if (storeyDel != nullptr)
					{
						for (auto& roomDel : storeyDel->_rooms)//кімнати
						{
							if (roomDel != nullptr)
							{
								for (auto& bookingClientsDel : roomDel->_bookingHistoryClients)
								{
									if (bookingClientsDel != nullptr)
									{
										delete bookingClientsDel;
										bookingClientsDel = nullptr;
									}
								}
								for (auto& bookingCompaniesDel : roomDel->_bookingHistoryCompanies)
								{
									if (bookingCompaniesDel != nullptr)
									{
										delete bookingCompaniesDel;
										bookingCompaniesDel = nullptr;
									}
								}
								for (auto& spendingDel : roomDel->_spendingOnRoom)
								{
									if (spendingDel != nullptr)
									{
										delete spendingDel;
										spendingDel = nullptr;
									}
								}
								delete roomDel;
								roomDel = nullptr;
							}
						}
						delete storeyDel;
						storeyDel = nullptr;
					}
				}
				delete corpsDel;
				corpsDel = nullptr;
			}
		}
		for (auto& CompanyDel : HC->_companies)
		{
			if (CompanyDel != nullptr)
			{
				for (auto& CompanyBookHistoryDel : CompanyDel->_bookingCompany)
				{
					if (CompanyBookHistoryDel != nullptr)
					{
						delete CompanyBookHistoryDel;
						CompanyBookHistoryDel = nullptr;
					}
				}
				delete CompanyDel;
				CompanyDel = nullptr;
			}
		}
		for (auto& ClientDel : HC->_Clients)
		{
			if (ClientDel != nullptr)
			{
				for (auto& ClientBookHistoryDel : ClientDel->_bookingClient)
					if (ClientBookHistoryDel != nullptr)
					{
						delete ClientBookHistoryDel;
						ClientBookHistoryDel = nullptr;
					}
				delete ClientDel;
				ClientDel = nullptr;
			}
		}
		delete HC;
		HC = nullptr;
	}
}


void InitHistoryOfRoom(hotel_complex* HC, string pathToClientsFile, string pathToCompanyFile = "")
{
	//для файла з клієнтами
	ifstream outDataClients;
	string DataFromFileClients;
	outDataClients.open(pathToClientsFile);
	if (outDataClients.is_open())
	{
		vector<string> tempVector;
		while (!outDataClients.eof())
		{
			std::getline(outDataClients, DataFromFileClients);

			DelimString("	", DataFromFileClients, tempVector);
			if (tempVector.size() < 7)
				break;

			for (corps* corps_el : HC->_corps)
			{
				if (corps_el->corps_number == stoi(tempVector[0]))
				{
					for (storey* storey_el : corps_el->_storeys)
					{
						for (room_in_hotel* room_el : storey_el->_rooms)
						{
							if (room_el->roomNumber == stoi(tempVector[1]))
							{
								Booking_client* bk = new Booking_client;
								Booking_client* bkC = new Booking_client;//в клієнті
								ConvertToDate(".", tempVector[2], bkC->BookedFrom);
								ConvertToDate(".", tempVector[2], bk->BookedFrom);
								ConvertToDate(".", tempVector[3], bkC->BookedTo);
								ConvertToDate(".", tempVector[3], bk->BookedTo);

								bkC->client.surname = bk->client.surname = tempVector[4];
								bkC->client.name = bk->client.name = tempVector[5];
								bkC->corpsNumber = stoi(tempVector[0]);
								bkC->roomNumber = stoi(tempVector[1]);

								if (tempVector[6] == "yes")
									bk->IsCanceled = true;
								else
									bk->IsCanceled = false;
								bkC->IsCanceled = bk->IsCanceled;
								bool ClientIsAlreadyIn = false;
								for (auto& a : HC->_Clients)
								{
									if (a->name == tempVector[5] && a->surname == tempVector[4])//якщо клієнт вже був зареєстрований
									{
										ClientIsAlreadyIn = true;
										a->_bookingClient.push_back(bkC);
										break;
									}

								}
								if (!ClientIsAlreadyIn)//додавання нового клієнта в список клієнтів
								{

									Client* client = new Client;
									client->name = tempVector[5];
									client->surname = tempVector[4];
									client->_bookingClient.push_back(bkC);
									HC->_Clients.push_back(client);
								}
								room_el->_bookingHistoryClients.push_back(bk);
								goto tobreak;
							}
						}
					}
				}
			}
		tobreak:
			tempVector.clear();
		}
	}
	else
	{
		std::cout << "Помилка пiд час вiдкривання файла з даними про клiєнтiв!" << endl;
		ErrorAppeared = true;
	}
	//для організацій
	ifstream outDataCompanies;
	string DataFromFileCompanies;
	outDataCompanies.open(pathToCompanyFile);
	if (outDataCompanies.is_open())
	{
		vector<string> tempVector2;
		while (!outDataCompanies.eof())
		{
			std::getline(outDataCompanies, DataFromFileCompanies);
			DelimString("	", DataFromFileCompanies, tempVector2);
			if (tempVector2.size() < 10)
			{
				break;
			}
			for (corps* corps_el : HC->_corps)
			{
				if (corps_el->corps_number == stoi(tempVector2[1]))
				{
					for (storey* storey_el : corps_el->_storeys)
					{
						for (room_in_hotel* room_el : storey_el->_rooms)
						{
							if (room_el->roomNumber == stoi(tempVector2[5]))
							{
								Booking_company* bk = new Booking_company;
								Booking_company* bkC = new Booking_company;
								ConvertToDate(".", tempVector2[6], bkC->BookedFrom);
								ConvertToDate(".", tempVector2[6], bk->BookedFrom);
								ConvertToDate(".", tempVector2[7], bkC->BookedTo);
								ConvertToDate(".", tempVector2[7], bk->BookedTo);


								bkC->company.name = bk->company.name = tempVector2[8];
								if (tempVector2[9] == "yes")
									bk->IsCanceled = true;
								else
									bk->IsCanceled = false;
								bkC->IsCanceled = bk->IsCanceled;
								bkC->corpsNumber = stoi(tempVector2[1]);
								bkC->roomNumber = stoi(tempVector2[5]);

								for (auto& a : HC->_companies)
								{
									if (a->name == tempVector2[8])
									{
										a->_bookingCompany.push_back(bkC);
										break;
									}
								}
								room_el->_bookingHistoryCompanies.push_back(bk);
								goto tobreak2;
							}
						}
					}
				}
			}
		tobreak2:
			tempVector2.clear();
		}
	}
	else
	{
		std::cout << "Помилка пiд час вiдкривання файла з даними про органiзацiї!" << endl;
		ErrorAppeared = true;
	}
}

void InitExtraService(hotel_complex* HC, string path) //служба побуту
{
	ifstream f;
	string data;
	f.open(path);
	vector<string> tempVector;
	if (f.is_open())
	{
		while (!f.eof())
		{
			std::getline(f, data);
			DelimString("	", data, tempVector);
			if (tempVector.size() < 3)
			{
				break;
			}
			for (auto& corpsEl : HC->_corps)
			{
				if (corpsEl->corps_number == stoi(tempVector[0]))
				{
					additionalService tempVar;
					tempVar.name = tempVector[1];
					if (tempVector[2] == "включено")
					{
						tempVar.isIncluded = true;
					}
					else
					{
						if (tempVector[2] != "-")

							tempVar.price = stoi(tempVector[2]);
						else
							tempVar.price = -1;//якщо не має ціни за годину

						//if (tempVector[3] != "-")
						//	tempVar.pricePerDay = stoi(tempVector[3]);
						//else
						//	tempVar.pricePerDay = -1;//якщо не має ціни за день
					}

					corpsEl->_additionalServices.push_back(tempVar);
				}

			}

			tempVector.clear();
		}
	}
	else
	{
		ErrorAppeared = true;
		std::cout << "Помилка пiд час вiдкривання файлу з додатковими послугами!" << endl;
	}
}

void InitContracts(hotel_complex* HC, string path)
{
	ifstream f;
	string data;
	f.open(path);
	vector<string> tempVector;
	if (f.is_open())
	{
		while (!f.eof())
		{
			std::getline(f, data);
			if (data == "")
				break;

			DelimString("	", data, tempVector);
			Company* comp = new Company;
			vector<string> tempVector2;

			DelimString(".", tempVector[1], tempVector2);
			comp->contractFrom[0] = stoi(tempVector2[0]);
			comp->contractFrom[1] = stoi(tempVector2[1]);
			comp->contractFrom[2] = stoi(tempVector2[2]);
			tempVector2.clear();
			DelimString(".", tempVector[2], tempVector2);
			comp->contractTo[0] = stoi(tempVector2[0]);
			comp->contractTo[1] = stoi(tempVector2[1]);
			comp->contractTo[2] = stoi(tempVector2[2]);
			comp->name = tempVector[0];
			HC->_companies.push_back(comp);
			tempVector.clear();
			tempVector2.clear();
		}
	}
	else
	{
		ErrorAppeared = true;
		std::cout << "Помилка пiд час вiдкривання файлу з контрактами!" << endl;
	}
}

void InitComplaint(hotel_complex* HC, string path)
{
	ifstream f;
	string data;
	f.open(path);
	vector<string> tempVector;
	if (f.is_open())
	{
		while (!f.eof())
		{
			std::getline(f, data);
			if (data == "")
				break;

			DelimString("	", data, tempVector);
			bool clientWasAlready = false;
			for (auto& client : HC->_Clients)
			{
				if (client->name == tempVector[1] && client->surname == tempVector[0])
				{
					client->_complaint.push_back(tempVector[2]);
					clientWasAlready = true;
					break;
				}
			}
			if (!clientWasAlready)
			{
				Client* client = new Client;
				client->name = tempVector[1];
				client->surname = tempVector[0];
				client->_complaint.push_back(tempVector[2]);
				HC->_Clients.push_back(client);
			}
			tempVector.clear();
		}
	}
	else {
		ErrorAppeared = true;
		std::cout << "Помилка пiд час вiдкривання файлу з скаргами!" << endl;
	}
}

void InitExtraServicesClients(hotel_complex* HC, string path)
{
	ifstream f;
	string data;
	f.open(path);
	vector<string> tempVector;
	if (f.is_open())
	{
		while (!f.eof())
		{
			std::getline(f, data);
			if (data == "")
				break;

			DelimString("	", data, tempVector);

			for (auto& client : HC->_Clients)
			{
				if (client->name == tempVector[3] && client->surname == tempVector[4])
				{
					additionalServiceUsed ad;
					ad.corpsnumber = stoi(tempVector[0]);
					ad.name = tempVector[1];
					for (auto corpsEl : HC->_corps)
					{
						if (corpsEl->corps_number == ad.corpsnumber)
						{
							for (auto additionalServiceEl : corpsEl->_additionalServices)
							{
								if (additionalServiceEl.name == ad.name)
								{

									ad.isIncluded = additionalServiceEl.isIncluded;
									ad.price = additionalServiceEl.price;
								}
							}
						}
					}
					vector<string>tempVector2;
					DelimString(".", tempVector[2], tempVector2);
					ad.date[0] = stoi(tempVector2[0]);
					ad.date[1] = stoi(tempVector2[1]);
					ad.date[2] = stoi(tempVector2[2]);
					client->_additionalService.push_back(ad);
					tempVector2.clear();
					break;
				}
			}
			tempVector.clear();
		}
	}
	else {
		ErrorAppeared = true;
		std::cout << "Помилка пiд час вiдкривання файлу з додатковими послугами, якими скористалися клiєнти!" << endl;
	}
}


void InitSpendings(hotel_complex* HC, string path) // витрати
{
	ifstream f;
	string data;
	f.open(path);
	vector<string> tempVector;
	if (f.is_open())
	{
		while (!f.eof())
		{
			std::getline(f, data);
			if (data == "")
				break;
			DelimString("	", data, tempVector);
			for (auto& corpsEl : HC->_corps)
			{
				for (auto& storeyEl : corpsEl->_storeys)
				{
					for (auto& roomEl : storeyEl->_rooms)
					{
						if (corpsEl->corps_number == stoi(tempVector[0]) && roomEl->roomNumber == stoi(tempVector[1]))
						{
							spendingOnRoom* spendingTemp = new spendingOnRoom;
							ConvertToDate(".", tempVector[2], spendingTemp->dateOfPayment);
							spendingTemp->costs = stoi(tempVector[3]);
							roomEl->_spendingOnRoom.push_back(spendingTemp);
						}
					}
				}
			}
			tempVector.clear();
		}
	}
	else {
		ErrorAppeared = true;
		std::cout << "Помилка пiд час вiдкривання файлу з витратами!" << endl;
	}
}


//							REQUESTS					//

//							1							//
void request1(hotel_complex* HC, int mincountRooms)// за весь період
{
	vector<string> _companies;
	vector<size_t> _countPlace;
	size_t tempCount = 0;
	size_t countCompanies = 0;
	size_t maxLengthNames = 11;// "назва фірми" - довжина 11 
	for (auto& companyEl : HC->_companies)
	{
		for (auto& companyBooking : companyEl->_bookingCompany)
		{
			if (companyBooking->IsCanceled == false)
				tempCount++;
		}
		if (tempCount > mincountRooms || tempCount == mincountRooms)
		{
			_companies.push_back(companyEl->name);
			_countPlace.push_back(tempCount);
			if (companyEl->name.length() > maxLengthNames)
				maxLengthNames = companyEl->name.length();
			countCompanies++;
		}
		else
		{
			if (tempCount > 0 && mincountRooms <= 0)
			{
				_companies.push_back(companyEl->name);
				_countPlace.push_back(tempCount);
				if (companyEl->name.length() > maxLengthNames)
					maxLengthNames = companyEl->name.length();
				countCompanies++;
			}
		}
		tempCount = 0;
	}
	std::cout << "Загальна кiлькiсть фiрм, якi забронювали к-сть мiсць " << mincountRooms
		<< " або бiльше: " << countCompanies << endl;
	if (countCompanies != 0)
	{
		std::cout << setw(maxLengthNames) << "назва фiрми" << "   кiлькiсть мiсць" << endl;
		for (size_t i = 0; i < _companies.size(); i++)
		{
			std::cout << setw(maxLengthNames) << _companies[i] << "   " << _countPlace[i] << endl;
		}
	}
}
void request1(hotel_complex* HC, int mincountRooms, int* dateFrom, int* dateTo)//за певний час
{
	vector<string> _companies;
	vector<size_t> _countPlace;
	size_t tempCount = 0;
	size_t countCompanies = 0;
	size_t maxLengthNames = 11;// назва фірми - довжина 11 
	for (auto& companyEl : HC->_companies)
	{
		for (auto& companyBooking : companyEl->_bookingCompany)//к-сть нескасованих бронювань
		{
			if (companyBooking->IsCanceled == false &&
				DateBetweenOrEqual(dateFrom, companyBooking->BookedFrom, dateTo))
				tempCount++;
		}
		if (tempCount > mincountRooms || tempCount == mincountRooms)
		{
			_companies.push_back(companyEl->name);
			_countPlace.push_back(tempCount);
			if (companyEl->name.length() > maxLengthNames)
				maxLengthNames = companyEl->name.length();
			countCompanies++;
		}
		else
		{
			if (tempCount > 0 && mincountRooms <= 0)
			{
				_companies.push_back(companyEl->name);
				_countPlace.push_back(tempCount);
				if (companyEl->name.length() > maxLengthNames)
					maxLengthNames = companyEl->name.length();
				countCompanies++;
			}
		}
		tempCount = 0;
	}
	std::cout << "Загальна кiлькiсть фiрм, якi забронювали к-сть мiсць " << mincountRooms
		<< " або бiльше з " << dateFrom[0] << "." << dateFrom[1] << "." << dateFrom[2]
		<< " по " << dateTo[0] << "." << dateTo[1] << "." << dateTo[2] << " рiвна "
		<< countCompanies << endl;
	if (countCompanies != 0)
	{
		std::cout << setw(maxLengthNames) << "назва фiрми" << "   кiлькiсть мiсць" << endl;
		for (size_t i = 0; i < _companies.size(); i++)
		{
			std::cout << setw(maxLengthNames) << _companies[i] << "   " << _countPlace[i] << endl;
		}
	}
}
//							2							//
namespace forRequest2
{
	//чи клієнт бронював кімнату в період з dateFrom до dateTo та не відмінив бронь
	bool IsGuestInPeriod(Client* cl, int* dateFrom, int* dateTo)
	{
		for (auto BookingEl : cl->_bookingClient)
		{
			if (DateBetweenOrEqual(dateFrom, BookingEl->BookedFrom, dateTo))
			{
				if (!(BookingEl->IsCanceled))
					return 1;
			}
		}
		return 0;
	}
	// скільки разів клієнт зарезервовував кімнату та не відміняв бронь 
	size_t countVisitsInPeriod(Client* cl, int* dateFrom, int* dateTo)
	{
		size_t CountsVisit = 0;
		for (auto BookingEl : cl->_bookingClient)
		{
			if (DateBetweenOrEqual(dateFrom, BookingEl->BookedFrom, dateTo) && !(BookingEl->IsCanceled))
				CountsVisit++;
		}
		return CountsVisit;
	}

}
void request2(hotel_complex* HC, vector<string> tempVector, int* dateFrom, int* dateTo)
{
	/*
		tempVector - вектор з даними про клієнта
		tempVector[0] - прізвище клієнта, якщо не має то "-1"
		tempVector[1] - ім'я, якщо не має то "-1"
		tempVector[2] - кількість відвідувань за певний період, якщо не має то "-1"
	*/
	size_t countClients = 0;
	vector<Client*> _ClientsTemp = {};
	size_t numberOfElement = 0;
	size_t maxLengthName = 3;// "iм'я"
	size_t maxLengthSurname = 8;// "прізвище"
	for (size_t i = 0; i < tempVector.size(); i++)
	{
		if (tempVector[i] != "-1")
		{
			switch (i)
			{
			case 0://прізвище
				for (auto clEl : HC->_Clients)
				{
					if (clEl->surname == tempVector[i]
						&& forRequest2::IsGuestInPeriod(clEl, dateFrom, dateTo))
					{
						_ClientsTemp.push_back(clEl);
						countClients++;
					}
				}
				break;
			case 1://ім'я
				if (_ClientsTemp.empty())
				{
					for (auto clEl : HC->_Clients)
					{
						if (clEl->name == tempVector[i] && forRequest2::IsGuestInPeriod(clEl, dateFrom, dateTo))
						{
							_ClientsTemp.push_back(clEl);
							countClients++;
						}
					}
				}
				else
				{
					int j = 0;// для ітерації
					for (auto clEl : _ClientsTemp)
					{
						if (clEl->name != tempVector[i])//забирання зайвих даних, які не підходять по іменам
						{
							_ClientsTemp.erase(_ClientsTemp.begin() + j);
							countClients--;
						}
						j++;
					}
				}
				break;

			case 2://кількість відвідувань протягом часу dateFrom до dateTo 
				if (_ClientsTemp.empty())
				{
					for (auto clEl : HC->_Clients)
					{
						if (forRequest2::countVisitsInPeriod(clEl, dateFrom, dateTo) == stoi(tempVector[i])
							&& forRequest2::IsGuestInPeriod(clEl, dateFrom, dateTo))
						{
							_ClientsTemp.push_back(clEl);
							countClients++;
						}
					}
				}
				else
				{
					int j = 0;
					for (auto clEl : _ClientsTemp)
					{
						if (forRequest2::countVisitsInPeriod(clEl, dateFrom, dateTo) != stoi(tempVector[i]))
						{
							_ClientsTemp.erase(_ClientsTemp.begin() + j);
							countClients--;
						}
						j++;
					}
				}
				break;
			}
		}
	}
	for (auto a : _ClientsTemp)
	{
		if (a->name.length() > maxLengthName)
			maxLengthName = a->name.length();
		if (a->surname.length() > maxLengthSurname)
			maxLengthSurname = a->surname.length();
	}
	std::cout << "Кiлькiсть клiєнтiв, якi пiдходять за характеристиками: " << countClients << endl;
	if (countClients > 0)
	{
		std::cout << setw(maxLengthName) << "iм'я" << "  " << setw(maxLengthSurname) << "прiзвище" << endl;
		for (auto cl : _ClientsTemp)
			std::cout << setw(maxLengthName) << cl->name << "  " << setw(maxLengthSurname) << cl->surname << endl;
	}
}
//							3							//
void request3(hotel_complex* HC, int* currentDate)
{
	size_t Count = 0;
	for (auto corps : HC->_corps)
	{
		for (auto storey : corps->_storeys)
		{
			for (auto room : storey->_rooms)
			{
				bool isFree = true;
				for (auto BookingRoomClient : room->_bookingHistoryClients)
				{
					if (!(BookingRoomClient->IsCanceled) &&
						DateBetweenOrEqual(BookingRoomClient->BookedFrom,
							currentDate, BookingRoomClient->BookedTo))//якщо не скасована
					{
						isFree = false;
						break;
					}
				}
				for (auto BookingRoomCompany : room->_bookingHistoryCompanies)
				{
					if (!(BookingRoomCompany->IsCanceled) &&
						DateBetweenOrEqual(BookingRoomCompany->BookedFrom,
							currentDate, BookingRoomCompany->BookedTo))
					{
						isFree = false;
						break;
					}
				}
				if (isFree) Count++;
			}
		}
	}
	std::cout << "Кiлькiсть кiмнат вiльних на даний момент рiвна " << Count << endl;
}
//							4							//
void request4(hotel_complex* HC, vector<string> characteristics, int* currentDate)
{
	/*
		characteristics - вектор з даними про кімнату
		characteristics[0] - зірковість корпусу
		characteristics[1] - місцевість
		characteristics[2] - номер корпусу
	*/
	size_t Count = 0;
	vector<room_in_hotel*> _roomsWithCharacteristics = {};
	vector<size_t> _CorpsNumber;
	vector<room_in_hotel*> temp = {};
	for (int i = 0; i < characteristics.size(); i++)
	{
		if (characteristics[i] != "-1")
		{
			switch (i)
			{
			case 0://к-сть зірок
				for (auto corpsEl : HC->_corps)
				{
					if (corpsEl->star_rating == stoi(characteristics[i]))
					{
						for (auto storey : corpsEl->_storeys)
						{
							for (auto room : storey->_rooms)
							{
								bool isFree = true;
								for (auto BookingRoomClient : room->_bookingHistoryClients)
								{
									if (!(BookingRoomClient->IsCanceled) &&
										DateBetweenOrEqual(BookingRoomClient->BookedFrom,
											currentDate, BookingRoomClient->BookedTo))//якщо не скасована
									{
										isFree = false;
										break;
									}
								}
								for (auto BookingRoomCompany : room->_bookingHistoryCompanies)
								{
									if (!(BookingRoomCompany->IsCanceled) &&
										DateBetweenOrEqual(BookingRoomCompany->BookedFrom,
											currentDate, BookingRoomCompany->BookedTo))
									{
										isFree = false;
										break;
									}
								}
								if (isFree) _roomsWithCharacteristics.push_back(room);
								_CorpsNumber.push_back(corpsEl->corps_number);

							}
						}
					}
				}
				break;
			case 1:// місцевість
				if (_roomsWithCharacteristics.empty())
				{
					for (auto corpsEl : HC->_corps)
					{
						for (auto storey : corpsEl->_storeys)
						{

							for (auto room : storey->_rooms)
							{
								if (room->capacity == stoi(characteristics[i]))
								{
									bool isFree = true;
									for (auto BookingRoomClient : room->_bookingHistoryClients)
									{
										if (!(BookingRoomClient->IsCanceled) &&
											DateBetweenOrEqual(BookingRoomClient->BookedFrom,
												currentDate, BookingRoomClient->BookedTo))//якщо не скасована
										{
											isFree = false;
											break;
										}
									}
									for (auto BookingRoomCompany : room->_bookingHistoryCompanies)
									{
										if (!(BookingRoomCompany->IsCanceled) &&
											DateBetweenOrEqual(BookingRoomCompany->BookedFrom,
												currentDate, BookingRoomCompany->BookedTo))
										{
											isFree = false;
											break;
										}
									}
									if (isFree) _roomsWithCharacteristics.push_back(room);
									_CorpsNumber.push_back(corpsEl->corps_number);
								}
							}
						}
					}
				}
				else
				{
					for (size_t it = 0; it < _roomsWithCharacteristics.size(); it++)
					{
						if (_roomsWithCharacteristics[it]->capacity != stoi(characteristics[i]))
						{
							_roomsWithCharacteristics.erase(_roomsWithCharacteristics.begin() + it);
							it--;
						}
					}
				}
				break;
			case 2:// номер корпусу
				if (_roomsWithCharacteristics.empty())
				{
					for (auto corpsEl : HC->_corps)
					{
						if (corpsEl->corps_number == stoi(characteristics[i]))
						{
							for (auto storey : corpsEl->_storeys)
							{

								for (auto room : storey->_rooms)
								{

									bool isFree = true;
									for (auto BookingRoomClient : room->_bookingHistoryClients)
									{
										if (!(BookingRoomClient->IsCanceled) &&
											DateBetweenOrEqual(BookingRoomClient->BookedFrom,
												currentDate, BookingRoomClient->BookedTo))//якщо не скасована
										{
											isFree = false;
											break;
										}
									}
									for (auto BookingRoomCompany : room->_bookingHistoryCompanies)
									{
										if (!(BookingRoomCompany->IsCanceled) &&
											DateBetweenOrEqual(BookingRoomCompany->BookedFrom,
												currentDate, BookingRoomCompany->BookedTo))
										{
											isFree = false;
											break;
										}
									}
									if (isFree) _roomsWithCharacteristics.push_back(room);
									_CorpsNumber.push_back(corpsEl->corps_number);
								}
							}
						}
					}
				}
				else
				{
					for (size_t it = 0; it < _roomsWithCharacteristics.size(); it++)
					{
						if (_CorpsNumber[it] != stoi(characteristics[i]))
						{
							_roomsWithCharacteristics.erase(_roomsWithCharacteristics.begin() + it);
							it--;
						}
					}
				}
				break;
			}
		}
	}
	std::cout << "Кiлькiсть вiльних кiмнат iз зазначеними характеристиками на даний момент рiвна " << _roomsWithCharacteristics.size() << endl;
}

//							5							//
void SortVectorDate(vector<vector<int>>& a)
{

	for (size_t i = 0; i < a.size(); i++)
	{
		for (size_t j = 0; j < a.size() - 1; j++)
		{
			if (!smallerOrEqual(a[j], a[j + 1]))
			{
				Date temp;
				temp[0] = a[j][0];
				temp[1] = a[j][1];
				temp[2] = a[j][2];

				a[j][0] = a[j + 1][0];
				a[j][1] = a[j + 1][1];
				a[j][2] = a[j + 1][2];

				a[j + 1][0] = temp[0];
				a[j + 1][1] = temp[1];
				a[j + 1][2] = temp[2];
			}
		}
	}
}

vector<int> DateBefore(vector<int> date)
{
	vector<int> dateBefore;
	if (date[0] == 1)
	{
		if (date[1] == 1)
		{
			dateBefore.push_back(31);
			dateBefore.push_back(12);
			dateBefore.push_back(date[2] - 1);
		}
		else
		{
			dateBefore.push_back(monthDays[date[1] - 1]);
			dateBefore.push_back(date[1] - 1);
			dateBefore.push_back(date[2]);
		}
	}
	else
	{
		dateBefore.push_back(date[0] - 1);
		dateBefore.push_back(date[1]);
		dateBefore.push_back(date[2]);
	}
	return dateBefore;
}

void request5(hotel_complex* HC, int numberCorps, int roomNumber)
{
	bool OK = false;// якщо кімната знайдено - true 
	/*vector<int> _currentDate;
	_currentDate.push_back(*currentDate);
	_currentDate.push_back(*(currentDate + 1));
	_currentDate.push_back(*(currentDate + 2));*/
	room_in_hotel* room = HC->_corps[0]->_storeys[0]->_rooms[0];
	size_t storeyNumber;
	vector<vector<int>> _bookingFrom_General;
	vector<vector<int>> _bookingTo_General;
	for (auto corpsEl : HC->_corps)
	{
		if (corpsEl->corps_number == numberCorps)
		{
			for (auto storeyEl : corpsEl->_storeys)
			{
				for (auto roomEl : storeyEl->_rooms)
				{
					if (roomEl->roomNumber == roomNumber)
					{

						for (auto bookingClientFrom : roomEl->_bookingHistoryClients)
						{
							if (!bookingClientFrom->IsCanceled)
							{
								vector<int> temp;
								temp.push_back(bookingClientFrom->BookedFrom[0]);
								temp.push_back(bookingClientFrom->BookedFrom[1]);
								temp.push_back(bookingClientFrom->BookedFrom[2]);

								_bookingFrom_General.push_back(temp);
							}
						}
						for (auto bookingCompanyFrom : roomEl->_bookingHistoryCompanies)
						{
							if (!bookingCompanyFrom->IsCanceled)
							{
								vector<int> temp;
								temp.push_back(bookingCompanyFrom->BookedFrom[0]);
								temp.push_back(bookingCompanyFrom->BookedFrom[1]);
								temp.push_back(bookingCompanyFrom->BookedFrom[2]);

								_bookingFrom_General.push_back(temp);
							}
						}


						for (auto bookingClientTo : roomEl->_bookingHistoryClients)
						{
							if (!bookingClientTo->IsCanceled)
							{
								vector<int> temp;
								temp.push_back(bookingClientTo->BookedTo[0]);
								temp.push_back(bookingClientTo->BookedTo[1]);
								temp.push_back(bookingClientTo->BookedTo[2]);

								_bookingTo_General.push_back(temp);
							}
						}
						for (auto bookingCompanyTo : roomEl->_bookingHistoryCompanies)
						{
							if (!bookingCompanyTo->IsCanceled)
							{
								vector<int> temp;
								temp.push_back(bookingCompanyTo->BookedTo[0]);
								temp.push_back(bookingCompanyTo->BookedTo[1]);
								temp.push_back(bookingCompanyTo->BookedTo[2]);

								_bookingTo_General.push_back(temp);
							}
						}
						OK = true;
						SortVectorDate(_bookingFrom_General);
						SortVectorDate(_bookingTo_General);
						room = roomEl;
						storeyNumber = storeyEl->storeyNumber;
						goto tobreak;
					}
					int a;

				}
			}
		}
	}
tobreak:
	if (OK)
	{
		std::cout << "поверх, на якому знаходиться цей номер: " << storeyNumber << endl;
		std::cout << "мiсцевiсть номера: " << room->capacity << endl;
		std::cout << "цiна за добу: " << room->pricePerDay << endl;
		if (_bookingFrom_General.empty())//якщо кiмната була завжди выльна
		{
			std::cout << "Ця кiмната завжди була вiльна!" << endl;
		}
		else
		{
			string history = "Вiльний вiд початку заснування готелю до " + to_string(_bookingFrom_General[0][0]) + "."
				+ to_string(_bookingFrom_General[0][1]) + "."
				+ to_string(_bookingFrom_General[0][2]) + " невключно;\n";
			for (size_t i = 0; i < _bookingFrom_General.size() - 1; i++)
			{
				if (getDifference(_bookingTo_General[i], _bookingFrom_General[i + 1]) > 2)
				{
					history += "вiд " + to_string(_bookingTo_General[i][0]) + "."
						+ to_string(_bookingTo_General[i][1]) + "."
						+ to_string(_bookingTo_General[i][2]) + " до "
						+ to_string(_bookingFrom_General[i + 1][0]) + "."
						+ to_string(_bookingFrom_General[i + 1][1]) + "."
						+ to_string(_bookingFrom_General[i + 1][2]) + " невключно;\n";
				}
				if (getDifference(_bookingTo_General[i], _bookingFrom_General[i + 1]) == 2)
				{
					vector<int> datebefore = DateBefore(_bookingFrom_General[i + 1]);
					history += "також вiльний " + to_string(datebefore[0]) + "."
						+ to_string(datebefore[1]) + "."
						+ to_string(datebefore[2]) + ";\n";
				}
			}
			history += "вiд " + to_string(_bookingTo_General[_bookingTo_General.size() - 1][0]) + "."
				+ to_string(_bookingTo_General[_bookingTo_General.size() - 1][1]) + "."
				+ to_string(_bookingTo_General[_bookingTo_General.size() - 1][2]) + " невключно та до кiнця iснування готелю.";
			std::cout << history << endl;
		}
	}
	else
		std::cout << "Такої кiмнати не має!" << endl;

}

//									6				//
bool IsFreeInPeriod(room_in_hotel* room, int* CurrentDay)
{
	for (auto bookingClient : room->_bookingHistoryClients)
	{
		if (!bookingClient->IsCanceled &&
			DateBetweenOrEqual(bookingClient->BookedFrom, CurrentDay, bookingClient->BookedTo))
			return false;

	}
	for (auto bookingCompany : room->_bookingHistoryCompanies)
	{
		if (!bookingCompany->IsCanceled &&
			DateBetweenOrEqual(bookingCompany->BookedFrom, CurrentDay, bookingCompany->BookedTo))
			return false;
	}
	return true;
}

void request6(hotel_complex* HC, int* currentDate, int* termin)
{
	if (!smallerOrEqual(termin, currentDate))
	{
		vector<pair<int, room_in_hotel*>> _rooms{};//1 - номер корпусу 2 - кімната
		bool isFreeFromPeriod = false;
		for (auto corpsEl : HC->_corps)
		{
			for (auto storeyEl : corpsEl->_storeys)
			{
				for (auto roomEl : storeyEl->_rooms)
				{
					if (!IsFreeInPeriod(roomEl, currentDate) && IsFreeInPeriod(roomEl, termin))//якщо зараз не вільний
					{
						pair<int, room_in_hotel*> tempRoom(corpsEl->corps_number, roomEl);
						_rooms.push_back(tempRoom);
					}
				}
			}
		}
		if (_rooms.size() != 0)
		{
			std::cout << "Список кiмнат: " << endl;
			std::cout << "Корпус		Номер кiмнати" << endl;
			for (auto el : _rooms)
			{
				std::cout << el.first << "		" << el.second->roomNumber << endl;
			}
		}
		if (_rooms.empty())
		{
			std::cout << "Кiлькiсть номерiв, якi зараз зайнятi та будуть вiльнi до зазначеного термiну рiвна 0!" << endl;
		}
	}
	else
	{
		cout << "Дата для вводу не може бути меншою або такою ж за поточну дату!" << endl;
	}
}
//									7				//
namespace request7Namespace
{
	struct roomForRequest7
	{
		size_t roomNumber;
		size_t corpsNumber;
		size_t CountOfBooking;// к-сть нескасованих
	};

	void SortVector(vector<request7Namespace::roomForRequest7>& vc)//сортує по CountOfBooking
	{
		for (size_t i = 0; i < vc.size(); i++)
		{
			for (size_t j = 0; j < vc.size() - i - 1; j++)
			{
				if (vc[j].CountOfBooking < vc[j + 1].CountOfBooking)
				{
					request7Namespace::roomForRequest7 temp = vc[j];
					vc[j] = vc[j + 1];
					vc[j + 1] = temp;
				}
			}
		}
	}
}
void request7(hotel_complex* HC, string nameCompany, int* terminFrom, int* terminTo)
{
	size_t generalCountOfBooking = 0;// загальна к-сть нескасованих
	vector<request7Namespace::roomForRequest7> _rooms;
	for (auto companyEl : HC->_companies)
	{
		if (companyEl->name == nameCompany)
		{
			for (auto booking : companyEl->_bookingCompany)
			{
				if (!booking->IsCanceled &&
					DateBetweenOrEqual(terminFrom, booking->BookedFrom, terminTo))
				{
					bool RoomIsAlreadyInVector = false;
					for (size_t i = 0; i < _rooms.size(); i++)
					{
						if (_rooms[i].roomNumber == booking->roomNumber && _rooms[i].corpsNumber == booking->corpsNumber)
						{
							_rooms[i].CountOfBooking++;
							RoomIsAlreadyInVector = true;
							generalCountOfBooking++;
						}
					}
					if (!RoomIsAlreadyInVector)
					{
						request7Namespace::roomForRequest7 tempRoom;
						tempRoom.corpsNumber = booking->corpsNumber;
						tempRoom.CountOfBooking = 1;
						tempRoom.roomNumber = booking->roomNumber;
						generalCountOfBooking++;
						_rooms.push_back(tempRoom);
					}
				}
			}
		}
	}
	request7Namespace::SortVector(_rooms);
	std::cout << "Загальна кiлькiсть бронювань цiєю фiрмою, якi не були скасованi за цей перiод: " << generalCountOfBooking << endl;
	if (generalCountOfBooking > 0)
	{
		std::cout << "Список бронювань:" << endl;
		std::cout << "Корпус	" << "	кiмната	" << "	кiлькiсть бронювань" << endl;
		for (auto el : _rooms)
		{
			std::cout << el.corpsNumber << "		" << el.roomNumber << "		" << el.CountOfBooking << endl;
		}
	}
}
//										8					//

void request8(hotel_complex* HC)
{
	bool isAtLeastOneComplaint = false;
	for (auto clientEl : HC->_Clients)
	{
		if (!clientEl->_complaint.empty())
		{
			isAtLeastOneComplaint = true;
			std::cout << "Прiзвище клiєнта: " << clientEl->surname << endl;
			std::cout << "Iм'я клiєнта: " << clientEl->name << endl;
			size_t numberOfComplaint = 1;
			for (auto complaintEl : clientEl->_complaint)
			{
				std::cout << "Скарга " << numberOfComplaint << ": " << complaintEl << endl;
				numberOfComplaint++;
			}
			std::cout << endl << endl;
		}
	}
	if (!isAtLeastOneComplaint) std::cout << "Не має жодної скарги!" << endl;
}
//									9				//

size_t GetProfitInPeriod(room_in_hotel* roomEl, int* dateFrom, int* dateTo)// скільки клієнт заплатив за декілька днів проживання
{
	Date dateFromTemp;
	dateFromTemp[0] = *dateFrom;
	dateFromTemp[1] = *(dateFrom + 1);
	dateFromTemp[2] = *(dateFrom + 2);
	Date dateToTemp;
	dateToTemp[0] = *dateTo;
	dateToTemp[1] = *(dateTo + 1);
	dateToTemp[2] = *(dateTo + 2);
	return (roomEl->pricePerDay * (getDifference(dateFromTemp, dateToTemp)));
}

void request9(hotel_complex* HC, int* TimeFrom, int* TimeTo)
{
	cout << "Корпус	Номер кiмнати	вiдношення" << endl;

	for (auto corpsEl : HC->_corps)
	{
		for (auto storeyEl : corpsEl->_storeys)
		{
			for (auto roomEl : storeyEl->_rooms)
			{
				double spendingInPeriod = 0;
				double profitInPeriod = 0;
				for (auto& spendingEl : roomEl->_spendingOnRoom)
				{
					if (DateBetweenOrEqual(TimeFrom, spendingEl->dateOfPayment, TimeTo))
						spendingInPeriod += spendingEl->costs;
				}
				for (auto& bookingCompanyEl : roomEl->_bookingHistoryCompanies)
				{
					if (bookingCompanyEl->IsCanceled == false && DateBetweenOrEqual(TimeFrom, bookingCompanyEl->BookedFrom, TimeTo))
					{
						Date dateFromTemp;
						dateFromTemp[0] = *TimeFrom;
						dateFromTemp[1] = *(TimeFrom + 1);
						dateFromTemp[2] = *(TimeFrom + 2);
						Date dateToTemp;
						dateToTemp[0] = *TimeTo;
						dateToTemp[1] = *(TimeTo + 1);
						dateToTemp[2] = *(TimeTo + 2);
						profitInPeriod += (GetProfitInPeriod(roomEl, bookingCompanyEl->BookedFrom, bookingCompanyEl->BookedTo) * discount / 100);
					}
				}
				for (auto& bookingClientsEl : roomEl->_bookingHistoryClients)
				{
					if (bookingClientsEl->IsCanceled == false && DateBetweenOrEqual(TimeFrom, bookingClientsEl->BookedFrom, TimeTo))
					{
						profitInPeriod += GetProfitInPeriod(roomEl, bookingClientsEl->BookedFrom, bookingClientsEl->BookedTo);
					}
				}
				std::cout << corpsEl->corps_number << "	" << roomEl->roomNumber << "	";
				if (spendingInPeriod == 0)
				{
					std::cout << "	Витрати рiвнi 0! Дiлити на 0 не можна!" << std::endl;
				}
				else
				{
					std::cout << "	" << profitInPeriod / spendingInPeriod << endl;
				}
			}
		}
	}
}
//								10							//
void request10(hotel_complex* HC, int corpsNumber, int roomNumber, int* currentDate)
{
	string surnameClient = "";
	string nameClient = "";
	bool OK = false;
	for (auto corpsEl : HC->_corps)
	{
		if (corpsEl->corps_number == corpsNumber)
		{
			for (auto storeyEl : corpsEl->_storeys)
			{
				for (auto roomEl : storeyEl->_rooms)
				{
					if (roomEl->roomNumber == roomNumber)
					{
						for (auto bookingEl : roomEl->_bookingHistoryClients)
						{
							if (!bookingEl->IsCanceled &&
								DateBetweenOrEqual(bookingEl->BookedFrom, currentDate, bookingEl->BookedTo))
							{
								surnameClient = bookingEl->client.surname;
								nameClient = bookingEl->client.name;
								OK = true;
								goto tobreak;
							}
						}
					}
				}
			}
		}
	}
tobreak:
	if (OK)// якщо знайшлася кiмната та клієнт забронював і не скасував бронь у зазначений час
	{
		size_t billForAdditionalService = 0;
		vector<string> _AdditionalServiceName{};
		vector<string> _complaints{};
		for (auto clientEl : HC->_Clients)
		{
			if (clientEl->name == nameClient && clientEl->surname == surnameClient)
			{
				for (auto ASU : clientEl->_additionalService)//additional service used
				{

					if (!ASU.isIncluded)
					{
						billForAdditionalService += ASU.price;
					}
					_AdditionalServiceName.push_back(ASU.name);
				}
				for (auto complaintEl : clientEl->_complaint)
				{
					_complaints.push_back(complaintEl);
				}
			}
		}
		std::cout << "Прiзвище: " << surnameClient << endl;
		std::cout << "Iм'я: " << nameClient << endl << endl;
		std::cout << "Рахунок готелю за всi додатковi послуги: " << billForAdditionalService << endl;
		if (!_AdditionalServiceName.empty())
		{
			std::cout << "Додатковi послуги, якими вiн користувався: " << endl;
		}
		for (size_t i = 0; i < _AdditionalServiceName.size(); i++)
		{
			std::cout << i + 1 << ". " << _AdditionalServiceName[i] << endl;
		}
		std::cout << endl;
		if (!_complaints.empty())
		{
			std::cout << "Скарги вiд цього користувача: " << endl;
			for (size_t i = 0; i < _complaints.size(); i++)
			{
				std::cout << i + 1 << ". " << _complaints[i] << endl;
			}
		}
		else
		{
			std::cout << "Скарг вiд цього клiєнта не має!" << endl;
		}

	}
	else
		std::cout << "Або в цiй кiмнатi не має зараз клiєнта, або такої кiмнати не iснує!" << endl;

}

//										11					//
namespace request11NameSpace
{
	struct CompanyForRequest11
	{
		Company* company = nullptr;
		size_t countOfNotCanceledBooking;
		size_t countOfNotCanceledBookingInPeriod;
	};
	size_t CountOfNotCanceledBooking(Company* company)
	{
		size_t count = 0;
		for (auto booking : company->_bookingCompany)
		{
			if (!booking->IsCanceled)
				count++;
		}
		return count;
	}
}

void request11(hotel_complex* HC, int* dateFrom, int* dateTo)
{
	vector<request11NameSpace::CompanyForRequest11> _companyWithBooking;
	for (auto companyEl : HC->_companies)
	{
		bool HasBooking = false;
		request11NameSpace::CompanyForRequest11 tempCompany;
		for (auto bookingEl : companyEl->_bookingCompany)
		{
			if (!bookingEl->IsCanceled &&
				DateBetweenOrEqual(dateFrom, bookingEl->BookedFrom, dateTo))
			{
				HasBooking = true;
				if (tempCompany.company == nullptr)
				{
					tempCompany.company = companyEl;
					tempCompany.countOfNotCanceledBookingInPeriod = 1;
				}
				else
					tempCompany.countOfNotCanceledBookingInPeriod++;

			}

		}
		if (HasBooking)
		{
			tempCompany.countOfNotCanceledBooking = request11NameSpace::CountOfNotCanceledBooking(tempCompany.company);
			_companyWithBooking.push_back(tempCompany);

		}
	}

	if (_companyWithBooking.empty())
	{
		std::cout << "Не має компанiй, якi забронювали кiмнати пiд час цього перiоду" << endl;
	}
	else
	{
		size_t i = 0;
		std::cout << "Кiлькiсть фiрм, з якими укладенi договори про бронь на зазначений перiод рiвна " << _companyWithBooking.size() << endl;

		for (auto companyEl : _companyWithBooking)
		{
			string contractFrom =
				to_string(companyEl.company->contractFrom[0]) + "." +
				to_string(companyEl.company->contractFrom[1]) + "." +
				to_string(companyEl.company->contractFrom[2]);
			string contractTo =
				to_string(companyEl.company->contractTo[0]) + "." +
				to_string(companyEl.company->contractTo[1]) + "." +
				to_string(companyEl.company->contractTo[2]);
			std::cout << "Назва фiрми: " << companyEl.company->name << endl;
			std::cout << "Договiр з фiрмою вiд " << contractFrom << " до " << contractTo << endl;
			std::cout << "Кiлькiсть нескасованих бронювань цiєю фiрмою протягом зазначеного перiоду: " << companyEl.countOfNotCanceledBookingInPeriod << endl;
			std::cout << "Загальна кiлькiсть нескасованих бронювань протягом всього перiоду спiвпрацi з фiрмою: " << companyEl.countOfNotCanceledBooking << endl;
			std::cout << "Загальна кiлькiсть бронювань даною фiрмою за весь час: " << companyEl.company->_bookingCompany.size() << endl;

			std::cout << endl << endl;
		}
	}

}

//										12						//

namespace request12Namespace
{
	size_t CountOfNotCanceledBooking(Client* cl)
	{
		size_t count = 0;
		for (auto bk : cl->_bookingClient)
		{
			if (!bk->IsCanceled) count++;
		}
		return count;
	}
	size_t CountOfNotCanceledBookingInCorps(Client* cl, int corpsNumber)
	{
		size_t count = 0;
		for (auto bk : cl->_bookingClient)
		{
			if (!bk->IsCanceled && bk->corpsNumber == corpsNumber) count++;
		}
		return count;
	}
	void SortVectorBy(vector<pair<int, Client*>>& vc) // сортує по к-сті відвідувань
	{
		for (size_t i = 0; i < vc.size(); i++)
		{
			for (size_t j = 0; j < vc.size() - i - 1; j++)
			{
				if (vc[j].first < vc[j + 1].first)
				{
					pair<int, Client*> temp = vc[j];
					vc[j] = vc[j + 1];
					vc[j + 1] = temp;
				}
			}
		}
	}
}

void request12InAllComplex(hotel_complex* HC, int CountOfClients)
{
	vector<pair<int, Client*>> _clients{};
	size_t MaxLengthSurname = 8;// "прізвище" - довжина 8
	size_t MaxLengthName = 3;//"ім'я" -  довжина 3
	if (CountOfClients <= 0)
		std::cout << "Кiлькiсть клiєнтiв не може бути менша, або рiвна нулю!" << endl;
	else
	{
		for (Client* clientEl : HC->_Clients)
		{
			pair<int, Client*> cl;
			cl.first = request12Namespace::CountOfNotCanceledBooking(clientEl);
			cl.second = clientEl;
			if (clientEl->name.length() > MaxLengthName)
				MaxLengthName = clientEl->name.length();
			if (clientEl->surname.length() > MaxLengthSurname)
				MaxLengthSurname = clientEl->surname.length();
			_clients.push_back(cl);
		}
		request12Namespace::SortVectorBy(_clients);
		size_t i = 0;
		size_t MaxLengthOfIterator = to_string(_clients.size() + 1).length();
		std::cout << setw(MaxLengthName + MaxLengthOfIterator) << "Iм'я" << "     " << setw(MaxLengthSurname) << "прiзвище" << "     " << "кiлькiсть нескасованих бронювань" << endl;
		for (auto cl : _clients)
		{
			if (i == CountOfClients)
				break;
			std::cout << setw(MaxLengthOfIterator) << i + 1 << " " << setw(MaxLengthName) << cl.second->name << "     " << setw(MaxLengthSurname) << cl.second->surname << "     " << cl.first << endl;
			i++;
		}
	}
}

void request12InCorps(hotel_complex* HC, int corpsNumber, int CountOfClients)
{
	bool CorpsExist = false;
	for (auto corpsEl : HC->_corps)
		if (corpsEl->corps_number == corpsNumber) CorpsExist = true;
	if (CorpsExist)
	{
		if (CountOfClients <= 0)
			std::cout << "Кiлькiсть клiєнтiв не може бути менша, або рiвна нулю!" << endl;
		else
		{
			vector<pair<int, Client*>> _clients{};
			size_t MaxLengthSurname = 8;// "прізвище" - довжина 8
			size_t MaxLengthName = 3;//"ім'я" -  довжина 3
			for (Client* clientEl : HC->_Clients)
			{
				pair<int, Client*> cl;
				cl.first = request12Namespace::CountOfNotCanceledBookingInCorps(clientEl, corpsNumber);
				cl.second = clientEl;
				if (clientEl->name.length() > MaxLengthName)
					MaxLengthName = clientEl->name.length();
				if (clientEl->surname.length() > MaxLengthSurname)
					MaxLengthSurname = clientEl->surname.length();
				_clients.push_back(cl);
			}
			request12Namespace::SortVectorBy(_clients);
			size_t i = 0;
			size_t MaxLengthOfIterator = to_string(_clients.size() + 1).length();
			std::cout << setw(MaxLengthName + MaxLengthOfIterator) << "Iм'я" << "     " << setw(MaxLengthSurname) << "прiзвище" << "     " << "кiлькiсть нескасованих бронювань" << endl;
			for (auto cl : _clients)
			{
				if (i == CountOfClients)
					break;
				std::cout << setw(MaxLengthOfIterator) << i + 1 << " " << setw(MaxLengthName) << cl.second->name << "     " << setw(MaxLengthSurname) << cl.second->surname << "     " << cl.first << endl;
				i++;
			}
		}
	}
	else
		std::cout << "Корпуса з таким номером не має!" << endl;
}

void request12InCorps(hotel_complex* HC, int corpsNumber)
{
	bool CorpsExist = false;
	for (auto corpsEl : HC->_corps)
		if (corpsEl->corps_number == corpsNumber) CorpsExist = true;
	if (CorpsExist)
	{

		vector<pair<int, Client*>> _clients{};
		size_t MaxLengthSurname = 8;// "прізвище" - довжина 8
		size_t MaxLengthName = 3;//"ім'я" -  довжина 3
		for (Client* clientEl : HC->_Clients)
		{
			pair<int, Client*> cl;
			cl.first = request12Namespace::CountOfNotCanceledBookingInCorps(clientEl, corpsNumber);
			cl.second = clientEl;
			if (clientEl->name.length() > MaxLengthName)
				MaxLengthName = clientEl->name.length();
			if (clientEl->surname.length() > MaxLengthSurname)
				MaxLengthSurname = clientEl->surname.length();
			_clients.push_back(cl);
		}
		request12Namespace::SortVectorBy(_clients);
		std::cout << "Кiлькiсть клiєнтiв: " << _clients.size() << endl;
		size_t i = 0;
		size_t MaxLengthOfIterator = to_string(_clients.size() + 1).length();
		std::cout << setw(MaxLengthName + MaxLengthOfIterator) << "Iм'я" << "     " << setw(MaxLengthSurname) << "прiзвище" << "     " << "кiлькiсть нескасованих бронювань" << endl;
		for (auto cl : _clients)
		{
			std::cout << setw(MaxLengthOfIterator) << i + 1 << " " << setw(MaxLengthName) << cl.second->name << "     " << setw(MaxLengthSurname) << cl.second->surname << "     " << cl.first << endl;
			i++;
		}
	}
	else
		std::cout << "Корпуса з таким номером не має!" << endl;
}

void request12InAllComplex(hotel_complex* HC)//по всім корпусам
{
	vector<pair<int, Client*>> _clients{};
	size_t MaxLengthSurname = 8;// "прізвище" - довжина 8
	size_t MaxLengthName = 3;//"ім'я" -  довжина 3
	for (Client* clientEl : HC->_Clients)
	{
		pair<int, Client*> cl;
		cl.first = request12Namespace::CountOfNotCanceledBooking(clientEl);
		cl.second = clientEl;
		if (clientEl->name.length() > MaxLengthName)
			MaxLengthName = clientEl->name.length();
		if (clientEl->surname.length() > MaxLengthSurname)
			MaxLengthSurname = clientEl->surname.length();
		_clients.push_back(cl);
	}
	request12Namespace::SortVectorBy(_clients);
	std::cout << "Кiлькiсть клiєнтiв: " << _clients.size() << endl;
	size_t i = 0;
	size_t MaxLengthOfIterator = to_string(_clients.size() + 1).length();
	std::cout << setw(MaxLengthName + MaxLengthOfIterator + 2) << "Iм'я" << "      " << setw(MaxLengthSurname) << "прiзвище" << "     " << "  кiлькiсть нескасованих бронювань" << endl;
	for (auto cl : _clients)
	{
		std::cout << setw(MaxLengthOfIterator) << i + 1 << " " << setw(MaxLengthName) << cl.second->name << "     " << setw(MaxLengthSurname) << cl.second->surname << "     " << cl.first << endl;
		i++;
	}
}

//										13				//
bool IsNewClientFromPeriod(Client* cl, int* DateFrom, int* DateTo)
{
	bool IsNew = false;
	for (auto booking : cl->_bookingClient)
	{
		if (!booking->IsCanceled &&
			(smallerOrEqual(booking->BookedFrom, DateFrom) || smallerOrEqual(DateTo, booking->BookedFrom)))
		{
			IsNew = false;
			break;
		}
		else
		{
			if (!booking->IsCanceled &&
				DateBetweenOrEqual(DateFrom, booking->BookedFrom, DateTo))
			{
				IsNew = true;
			}
		}
	}
	return IsNew;
}

void request13(hotel_complex* HC, int* DateFrom, int* DateTo)
{
	vector<Client*> _clients{};
	size_t MaxLengthOfName = 3; //"ім'я" - довжина 3
	size_t MaxLengthOfSurname = 8; //"прізвище" - довжина 3

	for (auto clientEl : HC->_Clients)
	{
		if (IsNewClientFromPeriod(clientEl, DateFrom, DateTo))
		{
			_clients.push_back(clientEl);
			if (clientEl->name.length() > MaxLengthOfName) MaxLengthOfName = clientEl->name.length();
			if (clientEl->surname.length() > MaxLengthOfSurname) MaxLengthOfSurname = clientEl->surname.length();
		}
	}
	size_t MaxLengthOfIterators = stoi(to_string(_clients.size() + 1));

	if (_clients.empty())
		std::cout << "Нових клiєнтiв за вказаний перiод не має!" << endl;
	else
	{
		std::cout << setw(MaxLengthOfName + MaxLengthOfIterators + 2) << "Iм'я" << "     " << setw(MaxLengthOfSurname)
			<< "прiзвище" << "     " << endl;
		size_t i = 0;
		for (auto clEl : _clients)
		{
			std::cout << setw(MaxLengthOfIterators) << i + 1 << ". " << setw(MaxLengthOfName) << clEl->name
				<< "     " << setw(MaxLengthOfSurname)
				<< clEl->surname << endl;
			i++;
		}
	}
}

//										14				//
namespace request14NameSpace
{
	size_t CountOfNotCanceledBooking(Client* cl)
	{
		size_t count = 0;
		for (auto bk : cl->_bookingClient)
		{
			if (!bk->IsCanceled) count++;
		}
		return count;
	}
}

void request14(hotel_complex* HC, string name, string surname)
{
	Client* client = nullptr;
	for (auto clEl : HC->_Clients)
	{
		if (clEl->name == name && clEl->surname == surname) { client = clEl; break; }
	}
	if (client != nullptr)//якщо є такий користувач у системі
	{
		size_t CountVisits = client->_bookingClient.size();//загальна к-сть броней
		size_t countNotCanceledBooking = request14NameSpace::CountOfNotCanceledBooking(client);
		size_t billForAdditionalService = 0;
		vector<string> _AdditionalServiceName{};
		for (auto ASU : client->_additionalService)//additional service used
		{

			if (!ASU.isIncluded)
			{
				billForAdditionalService += ASU.price;
			}
			_AdditionalServiceName.push_back(ASU.name);
		}
		std::cout << "Рахунок цього клiєнта готелю за всi додатковi послуги: " << billForAdditionalService << endl;
		if (!_AdditionalServiceName.empty())
		{
			std::cout << "Додатковi послуги, якими вiн користувався: " << endl;
			for (size_t i = 0; i < _AdditionalServiceName.size(); i++)
			{
				std::cout << i + 1 << ". " << _AdditionalServiceName[i] << endl;
			}
		}
		size_t maxLengthCorps = 6;
		size_t maxLengthRoom = 7;
		size_t maxLengthBookingFrom = 10;
		size_t maxLengthBookingTo = 10;
		std::cout << "Загальна кiлькiсть бронювань цим клiєнтом: " << CountVisits << endl;
		std::cout << "Кiлькiсть нескасованих бронювань цим клiєнтом : " << countNotCanceledBooking << endl;

		std::cout << "Нескасованi бронювання даного клiєнта:" << endl;
		std::cout << "корпус" << "     " << "кiмната" << "      " << "броня вiд" << "      " << "броня до" << endl;
		for (auto booking : client->_bookingClient)
		{
			if (!booking->IsCanceled)
			{
				std::cout << setw(maxLengthCorps) << booking->corpsNumber << "     "
					<< setw(maxLengthRoom) << booking->roomNumber << "     "
					<< setw(maxLengthBookingFrom)
					<< to_string(booking->BookedFrom[0]) + "." + to_string(booking->BookedFrom[1]) + "." + to_string(booking->BookedFrom[2])
					<< "     " << setw(maxLengthBookingTo)
					<< to_string(booking->BookedTo[0]) + "." + to_string(booking->BookedTo[1]) + "." + to_string(booking->BookedTo[2]) << endl;
			}
		}
	}
	else
		std::cout << "Такого клiєнта в нас ще не було!" << endl;
}

//									15				//

//15. Отримати відомості про конкретний номер : ким він був зайнятий в певний період.
//

void request15(hotel_complex* HC, int corpsNumber, int roomNumber)
{
	vector<Booking_client*> clientsInThisRoom;
	vector<Booking_company*> companyInThisRoom;
	size_t MaxLengthCompanyName = 14;

	size_t MaxLengthClientName = 3;
	size_t MaxLengthClientSurname = 8;
	if (!CorpsExist(HC, corpsNumber)) std::cout << "Корпуса з таким номером не iснує!" << endl;

	else
	{
		if (!RoomExist(HC, corpsNumber, roomNumber)) std::cout << "Кiмнати з таким номером не iснує!" << endl;
		else
		{
			for (auto corpsEl : HC->_corps)
			{
				if (corpsEl->corps_number == corpsNumber)
				{
					for (auto storeyEl : corpsEl->_storeys)
					{
						for (auto roomEl : storeyEl->_rooms)
						{
							if (roomEl->roomNumber == roomNumber)
							{
								for (auto bookingClient : roomEl->_bookingHistoryClients)
								{
									if (!bookingClient->IsCanceled)
									{
										clientsInThisRoom.push_back(bookingClient);
										if (bookingClient->client.name.length() > MaxLengthClientName)
											MaxLengthClientName = bookingClient->client.name.length();
										if (bookingClient->client.surname.length() > MaxLengthClientSurname)
											MaxLengthClientSurname = bookingClient->client.surname.length();
									}
								}
								for (auto bookingCompany : roomEl->_bookingHistoryCompanies)
								{
									if (!bookingCompany->IsCanceled)
									{
										companyInThisRoom.push_back(bookingCompany);
										if (bookingCompany->company.name.length() > MaxLengthCompanyName)
											MaxLengthCompanyName = bookingCompany->company.name.length();
									}
								}
							}
						}
					}
				}
			}
			std::cout << "Кiлькiсть компанiй, якi забронювали цю кiмнату та не скасували бронь: " << companyInThisRoom.size() << endl;
			if (!companyInThisRoom.empty())
			{
				std::cout << "Компанiї, якi забронювали цей номер: " << endl;
				size_t MaxLengthOfIterator = to_string(companyInThisRoom.size()).length();
				size_t it = 1;
				std::cout << setw(MaxLengthCompanyName + MaxLengthOfIterator + 2) << "Назва компанiї" << "		"
					<< "броня вiд" << "	" << "броня до" << endl;
				for (auto bookingCompany : companyInThisRoom)
				{
					std::cout << setw(MaxLengthOfIterator) << it << ". " << setw(MaxLengthCompanyName) << bookingCompany->company.name <<
						"		" << to_string(bookingCompany->BookedFrom[0]) + "." +
						to_string(bookingCompany->BookedFrom[1]) + "." +
						to_string(bookingCompany->BookedFrom[2]) << "	"

						<< to_string(bookingCompany->BookedTo[0]) + "." +
						to_string(bookingCompany->BookedTo[1]) + "." +
						to_string(bookingCompany->BookedTo[2]) << endl;
					it++;
				}
			}
			///////////////////////////////////////
			std::cout << endl << "Кiлькiсть клiєнтiв, якi забронювали цю кiмнату та не скасували бронь: " << clientsInThisRoom.size() << endl;
			if (!clientsInThisRoom.empty())
			{
				std::cout << "Клiєнти, якi забронювали цей номер : " << endl;
				size_t MaxLengthOfIterator = to_string(clientsInThisRoom.size()).length();
				size_t it = 1;
				std::cout << setw(MaxLengthClientSurname + MaxLengthOfIterator + 2) << "Прiзвище" << "     " << setw(MaxLengthClientName)
					<< "Iм'я" << "		" << "броня вiд" << "	" << "броня до" << endl;
				for (auto bookingClient : clientsInThisRoom)
				{
					std::cout << setw(MaxLengthOfIterator) << it << ". " << setw(MaxLengthClientSurname) << bookingClient->client.surname <<
						"     " << setw(MaxLengthClientName) << bookingClient->client.name << "		" << to_string(bookingClient->BookedFrom[0]) + "." +
						to_string(bookingClient->BookedFrom[1]) + "." +
						to_string(bookingClient->BookedFrom[2]) << "	"

						<< to_string(bookingClient->BookedTo[0]) + "." +
						to_string(bookingClient->BookedTo[1]) + "." +
						to_string(bookingClient->BookedTo[2]) << endl;
					it++;
				}
			}
		}
	}

}
void request15(hotel_complex* HC, int corpsNumber, int roomNumber, int* dateFrom, int* dateTo)
{
	vector<Booking_client*> clientsInThisRoom;
	vector<Booking_company*> companyInThisRoom;
	size_t MaxLengthCompanyName = 14;

	size_t MaxLengthClientName = 3;
	size_t MaxLengthClientSurname = 8;
	if (!CorpsExist(HC, corpsNumber)) std::cout << "Корпуса з таким номером не iснує!" << endl;

	else
	{
		if (!RoomExist(HC, corpsNumber, roomNumber)) std::cout << "Кiмнати з таким номером не iснує!" << endl;
		else
		{
			for (auto corpsEl : HC->_corps)
			{
				if (corpsEl->corps_number == corpsNumber)
				{
					for (auto storeyEl : corpsEl->_storeys)
					{
						for (auto roomEl : storeyEl->_rooms)
						{
							if (roomEl->roomNumber == roomNumber)
							{
								for (auto bookingClient : roomEl->_bookingHistoryClients)
								{
									if (!bookingClient->IsCanceled &&
										DateBetweenOrEqual(dateFrom, bookingClient->BookedFrom, dateTo))
									{
										clientsInThisRoom.push_back(bookingClient);
										if (bookingClient->client.name.length() > MaxLengthClientName)
											MaxLengthClientName = bookingClient->client.name.length();
										if (bookingClient->client.surname.length() > MaxLengthClientSurname)
											MaxLengthClientSurname = bookingClient->client.surname.length();
									}
								}
								for (auto bookingCompany : roomEl->_bookingHistoryCompanies)
								{
									if (!bookingCompany->IsCanceled &&
										DateBetweenOrEqual(dateFrom, bookingCompany->BookedFrom, dateTo))
									{
										companyInThisRoom.push_back(bookingCompany);
										if (bookingCompany->company.name.length() > MaxLengthCompanyName)
											MaxLengthCompanyName = bookingCompany->company.name.length();
									}
								}
							}
						}
					}
				}
			}
			std::cout << "Кiлькiсть компанiй, якi забронювали цю кiмнату у вказаний перiод та не скасували бронь: " << companyInThisRoom.size() << endl;
			if (!companyInThisRoom.empty())
			{
				std::cout << "Компанiї, якi забронювали цей номер: " << endl;
				size_t MaxLengthOfIterator = to_string(companyInThisRoom.size()).length();
				size_t it = 1;
				std::cout << setw(MaxLengthCompanyName + MaxLengthOfIterator + 2) << "Назва компанiї" << "		"
					<< "броня вiд" << "	" << "броня до" << endl;
				for (auto bookingCompany : companyInThisRoom)
				{
					std::cout << setw(MaxLengthOfIterator) << it << ". " << setw(MaxLengthCompanyName) << bookingCompany->company.name <<
						"		" << to_string(bookingCompany->BookedFrom[0]) + "." +
						to_string(bookingCompany->BookedFrom[1]) + "." +
						to_string(bookingCompany->BookedFrom[2]) << "	"

						<< to_string(bookingCompany->BookedTo[0]) + "." +
						to_string(bookingCompany->BookedTo[1]) + "." +
						to_string(bookingCompany->BookedTo[2]) << endl;
					it++;
				}
			}
			///////////////////////////////////////
			std::cout << "Кiлькiсть клiєнтiв, якi забронювали цю кiмнату у вказаний перiод та не скасували бронь: " << clientsInThisRoom.size() << endl;
			if (!clientsInThisRoom.empty())
			{
				std::cout << "Клiєнти, якi забронювали цей номер : " << endl;
				size_t MaxLengthOfIterator = to_string(clientsInThisRoom.size()).length();
				size_t it = 1;
				std::cout << setw(MaxLengthClientSurname + MaxLengthOfIterator + 2) << "Прiзвище" << "     " << setw(MaxLengthClientName)
					<< "Iм'я" << "		" << "броня вiд" << "	" << "броня до" << endl;
				for (auto bookingClient : clientsInThisRoom)
				{
					std::cout << setw(MaxLengthOfIterator) << it << ". " << setw(MaxLengthClientSurname) << bookingClient->client.surname <<
						"     " << setw(MaxLengthClientName) << bookingClient->client.name << "		" << to_string(bookingClient->BookedFrom[0]) + "." +
						to_string(bookingClient->BookedFrom[1]) + "." +
						to_string(bookingClient->BookedFrom[2]) << "	"

						<< to_string(bookingClient->BookedTo[0]) + "." +
						to_string(bookingClient->BookedTo[1]) + "." +
						to_string(bookingClient->BookedTo[2]) << endl;
					it++;
				}
			}
		}
	}
}
//									16				//
//16. Отримати відсоткове відношення всіх номерів до номерів, заброньованих партнерами.
// (зараз

void request16(hotel_complex* HC, int* CurrentDate)
{
	size_t CountOfRoom = 0;
	size_t countOfBookingCompanyNow = 0;
	for (auto companyEl : HC->_companies)
	{
		for (auto bookingEl : companyEl->_bookingCompany)
		{
			if (!bookingEl->IsCanceled &&
				DateBetweenOrEqual(bookingEl->BookedFrom, CurrentDate, bookingEl->BookedTo))
			{
				countOfBookingCompanyNow++;
			}
		}
	}
	for (auto corpsEl : HC->_corps)
	{
		for (auto storeyEl : corpsEl->_storeys)
		{
			CountOfRoom += storeyEl->_rooms.size();
		}
	}
	if (CountOfRoom <= 0)
		std::cout << "Кiлькiсть кiмнат заброньованими фiрмами рiвна 0!" << std::endl;
	else
	{
		float ratio = ((float)countOfBookingCompanyNow / (float)CountOfRoom * 100);
		std::cout << "Загальна кiлькiсть кiмнат: " << CountOfRoom << endl;
		std::cout << "Кiлькiсть кiмнат, якi зараз заброньованi партнерами: " << countOfBookingCompanyNow << endl;
		std::cout << "Вiдношення загальної кiлькостi кiмнат до кiмнат заброньованої партнерами: "
			<< ratio << " %" << endl;
	}
}

//													//
bool YearIsLeap(string year)
{
	int yearInt = stoi(year);
	if (yearInt % 400 == 0) return 1;
	if (yearInt % 4 == 0 && yearInt % 100 != 0) return 1;
	return 0;
}
bool YearIsLeap(int yearInt)
{
	if (yearInt % 400 == 0) return 1;
	if (yearInt % 4 == 0 && yearInt % 100 != 0) return 1;
	return 0;
}
bool IsStringNumber(string str)
{
	size_t countOfDigits = 0;
	if (str[0] == '-')
	{
		countOfDigits++;
		for (size_t i = 1; i < str.length(); i++)
			if (str[i] >= '0' && str[i] <= '9') countOfDigits++;
	}
	else
	{
		for (size_t i = 0; i < str.length(); i++)
			if (str[i] >= '0' && str[i] <= '9') countOfDigits++;
	}
	if (countOfDigits == str.length()) return 1;
	else return 0;
}
bool StringIsRealDate(string dateToString, string delim)
{
	vector<string> tempVector{};
	DelimString(delim, dateToString, tempVector);
	if (tempVector.size() == 3)
	{
		//if (std::find(vec.begin(), vec.end(), value) != vec.end())
		if (IsStringNumber(tempVector[0]) && IsStringNumber(tempVector[1]) && IsStringNumber(tempVector[2]))
		{
			size_t day = stoi(tempVector[0]);
			size_t month = stoi(tempVector[1]);
			size_t year = stoi(tempVector[2]);

			if (month == 2 && YearIsLeap(year) && day <= 29)// високосний рік, 29 лютого
				return 1;
			if (month <= 12 && monthDays[month - 1] >= day)
			{
				return 1;
			}
		}
	}
	return 0;
}
void help(string& str)
{
	std::getline(std::cin, str);
	bool strIsNumber = false; //для перевірки чи рядок є цифрою від 1 до 16
	for (size_t i = 1; i <= 16; i++)
	{
		if (str == to_string(i)) strIsNumber = true;
	}
}
//bool IsStringNumberBetween(string str, int numberFrom, int numberTo)//чи рядок є числом в діапазоні від numberFrom до numberTo
//{
//	for (int i = numberFrom; i <= numberTo; i++)
//	{
//		if (str == to_string(i)) return true;
//	}
//	return false;
//}
void ChangeItoI(string& str)// для заміни української 'i' на англійську'і'
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == 'і')
			str[i] = 'i';
	}
}

//в dataInput буде записане значення вводу з консолі
//textInCout - текст, який має вивестися на консоль
//працює тільки з цілими числами
void CinCheckExceptionForNumbers(int& dataInput, string textInCout = "")
{
	bool stopLoop = false;
	int tempNumber;
	string strNumber;
	do
	{
		cout << textInCout;
		cin >> strNumber;
		if (IsStringNumber(strNumber))
		{
			stopLoop = true;
			dataInput = stoi(strNumber);
		}
		else
		{
			std::cout << "Введено неправлильно данi! Введи їх ще раз!" << endl;
		}
		_flushall();
		cin.ignore(2555, '\n');
	} while (!stopLoop);
}

void CinCheckExceptionForDates(string& dataInput, string textInCout = "")//перевірка чи ввід є датою в форматі dd.mm.yyyy
{
	bool stopLoop = false;
	int tempNumber;
	string strDate;
	do
	{
		cout << textInCout;
		cin >> strDate;
		if (StringIsRealDate(strDate, "."))
		{
			stopLoop = true;
			dataInput = strDate;
		}
		else
			std::cout << "Введено неправильнi данi! Введи правильнi данi!" << endl;
		_flushall();
		cin.ignore(2555, '\n');
	} while (!stopLoop);
}


void Execute(vector<string> _requestVector, int NumberOfRequest, int* CurrentDate)
{
	std::cout << "Вибрано запит № " << NumberOfRequest << endl;
	std::cout << "--------------------------------" << endl;
	std::cout << _requestVector[NumberOfRequest - 1] << endl;
	std::cout << "--------------------------------" << endl;
	string str;
	string choice1;
	string choice2;
	int minCountRooms;
	string dateFromString;
	string dateToString;
	Date dateTo;
	Date dateFrom;
	string tempString1;
	vector<string> tempVector;
	string surname;
	string name;
	string nameCompany;
	int countVisits;
	int countVisitsInPeriod;
	int CorpsStarRating;
	int capacityOfRoom;
	int numberOfCorps;
	int RoomNumber;
	int choice1Number;
	int choice2Number;
	bool correctchange = false;
	vector<string> _characteristics{};
	switch (NumberOfRequest)
	{
	case 1:
		std::cout << "Якщо бажаєш вибрати перелiк i загальне число фiрм," <<
			" що забронювали мiсця в обсязi, не менше вказаного, за весь " <<
			"перiод спiвпрацi напиши 1, якщо ж за деякий перiод спiвпрацi - напиши 2" << endl;
		while (!correctchange)
		{

			std::getline(std::cin, choice1);
			if (choice1 == "1")
			{
				correctchange = true;
				/*std::cout << "Введи мiнiмальну кiлькiсть кiмнат, яку забронювали компанiї(та не скасували бронь): ";
				cin >> minCountRooms;*/
				CinCheckExceptionForNumbers(minCountRooms,
					"Введи мiнiмальну кiлькiсть кiмнат, яку забронювали компанiї(та не скасували бронь): ");

				request1(HC, minCountRooms);
				std::cout << endl;
			}
			else
			{
				if (choice1 == "2")
				{
					correctchange = true;

					//std::cout << "Введи дату початку перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ";
					//std::getline(std::cin, dateFromString);
					CinCheckExceptionForDates(dateFromString, "Введи дату початку перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
					ConvertToDate(".", dateFromString, dateFrom);
					//	std::cout << "Введи дату кiнця перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ";
					//std::getline(std::cin, dateToString);
					CinCheckExceptionForDates(dateToString, "Введи дату кiнця перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
					ConvertToDate(".", dateToString, dateTo);
					//std::cout << "Введи мiнiмальну кiлькiсть кiмнат, яку забронювали компанiї(та не скасували бронь): ";
					//cin >> minCountRooms;
					CinCheckExceptionForNumbers(minCountRooms, "Введи мiнiмальну кiлькiсть кiмнат, яку забронювали компанiї(та не скасували бронь): ");
					request1(HC, minCountRooms, dateFrom, dateTo);
					std::cout << endl;

				}
				else
					std::cout << "Введено неправильну опцiю! Введи коректнi данi!" << endl;
			}
		}
		break;
	case 2:

		CinCheckExceptionForDates(dateFromString, "Введи дату початку перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateFromString, dateFrom);

		CinCheckExceptionForDates(dateToString, "Введи дату кiнця перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateToString, dateTo);
		cout << "Якщо бажаєш шукати клiєнта за його прiзвищем - введи його, \nякщо нi - введи '-1' : ";
		getline(std::cin, surname);
		tempVector.push_back(surname);
		cout << "Якщо бажаєш шукати клiєнта за його iм'ям - введи його, \nякщо нi - введи '-1' : ";
		getline(std::cin, name);
		tempVector.push_back(name);
		CinCheckExceptionForNumbers(countVisitsInPeriod,
			"Якщо бажаєш шукати клiєнта за кiлькiстю вiдвудувань за певний перiод - введи цю кiлькiсть, якщо нi - введи '-1': ");
		tempVector.push_back(to_string(countVisitsInPeriod));
		request2(HC, tempVector, dateFrom, dateTo);
		std::cout << endl;
		break;
	case 3:
		request3(HC, CurrentDate);
		std::cout << endl;
		break;

	case 4:
		CinCheckExceptionForNumbers(CorpsStarRating,
			"Якщо бажаєш шукати кiмнату за зiрковостю корпуса - введи кiлькiсть зiрок,\nяку має корпус, якщо нi - введи '-1' : ");
		_characteristics.push_back(to_string(CorpsStarRating));
		CinCheckExceptionForNumbers(capacityOfRoom,
			"Якщо бажаєш шукати кiмнату за її мiсцевiстю - введи її, якщо нi - введи '-1' : ");
		_characteristics.push_back(to_string(capacityOfRoom));

		CinCheckExceptionForNumbers(numberOfCorps,
			"Якщо бажаєш шукати кiмнату за номером корпусу - введи його, якщо нi - введи '-1' : ");
		_characteristics.push_back(to_string(numberOfCorps));
		request4(HC, _characteristics, CurrentDate);
		std::cout << endl;
		break;
	case 5:
		CinCheckExceptionForNumbers(RoomNumber, "Введи номер кiмнати: ");
		CinCheckExceptionForNumbers(numberOfCorps, "Введи номер корпуса в  якому знаходиться кiмната: ");
		request5(HC, numberOfCorps, RoomNumber);
		std::cout << endl;
		break;
	case 6:
		CinCheckExceptionForDates(dateToString,
			"Введи дату до якої зайнятi зараз номери мають звiльнитися в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateToString, dateTo);
		request6(HC, CurrentDate, dateTo);
		std::cout << endl;
		break;
	case 7:
		std::cout << "Введи назву фiрми: ";
		getline(std::cin, nameCompany);
		CinCheckExceptionForDates(dateFromString, "Введи дату початку перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateFromString, dateFrom);
		CinCheckExceptionForDates(dateToString, "Введи дату кiнця перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateToString, dateTo);
		request7(HC, nameCompany, dateFrom, dateTo);
		std::cout << endl;
		break;
	case 8:
		request8(HC);
		std::cout << endl;
		break;
	case 9:
		CinCheckExceptionForDates(dateFromString, "Введи дату початку перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateFromString, dateFrom);
		CinCheckExceptionForDates(dateToString, "Введи дату кiнця перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateToString, dateTo);
		request9(HC, dateFrom, dateTo);
		break;
	case 10:
		CinCheckExceptionForNumbers(RoomNumber, "Введи номер кiмнати: ");
		CinCheckExceptionForNumbers(numberOfCorps, "Введи номер корпуса, в  якому знаходиться кiмната: ");
		request10(HC, numberOfCorps, RoomNumber, CurrentDate);
		std::cout << endl;
		break;
	case 11:
		CinCheckExceptionForDates(dateFromString, "Введи дату початку перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateFromString, dateFrom);
		CinCheckExceptionForDates(dateToString, "Введи дату кiнця перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateToString, dateTo);
		request11(HC, dateFrom, dateTo);
		std::cout << endl;
		break;

	case 12:
		CinCheckExceptionForNumbers(choice1Number,
			"Якщо хочеш отримати вiдомостi про постояльцiв, якi бiльше за всiх вiдвiдують\n готель по всiм корпусам готельного комплексу введи '-1', якщо по певному корпусy введи номер цього корпусу : ");
		if (choice1Number == -1)
		{
			CinCheckExceptionForNumbers(choice2Number,
				"Якщо бажаєш отримати тiльки список певної кiлькостi клiєнтiв, то введи довжину цього списку, якщо ж нi - '-1': ");
			if (choice2Number == -1)
				request12InAllComplex(HC);
			else
				request12InAllComplex(HC, choice2Number);
		}
		else
		{
			CinCheckExceptionForNumbers(choice2Number,
				"Якщо бажаєш отримати тiльки список певної кiлькостi клiєнтiв, то введи довжину цього списку, якщо ж нi - '-1': ");
			if (choice2Number == -1)
				request12InCorps(HC, choice1Number);
			else
				request12InCorps(HC, choice1Number, choice2Number);
		}
		std::cout << endl;
		break;
	case 13:
		CinCheckExceptionForDates(dateFromString, "Введи дату початку перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateFromString, dateFrom);
		CinCheckExceptionForDates(dateToString, "Введи дату кiнця перiоду в форматi dd.mm.yyyy(наприклад 11.05.2021): ");
		ConvertToDate(".", dateToString, dateTo);
		request13(HC, dateFrom, dateTo);
		std::cout << endl;
		break;
	case 14:
		std::cout << "Введи прiзвище цiєї людини: ";
		//cin >> surname;
		getline(cin, surname);
		std::cout << "Введи iм'я цiєї людини: ";
		//cin >> name;
		getline(cin, name);
		request14(HC, name, surname);
		std::cout << endl;
		break;
	case 15:
		CinCheckExceptionForNumbers(RoomNumber, "Введи номер кiмнати: ");
		CinCheckExceptionForNumbers(numberOfCorps, "Введи номер корпуса, в  якому знаходиться кiмната: ");
		request15(HC, numberOfCorps, RoomNumber);
		std::cout << endl;
		break;
	case 16:
		request16(HC, CurrentDate);
		std::cout << endl;
		break;
	}

}

//				MAIN				///


int main()
{
	setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	string pathToDir;
	std::cout << "Введи шлях, де знаходиться папка з даними: ";
	getline(cin, pathToDir);
	for (int i = 0; i < pathToDir.size(); i++)
	{
		if (pathToDir[i] == '\\') { pathToDir[i] = '/'; }
	}
#ifdef _SIMPLE_TIMER_H
	//для розрахунку скільки часу потрібно програмі, щоби зчитати дані з файлів
	//та записати ці дані в еземпляр класу hotel_complex HC
	SimpleTimer* timeForReadingFiles = new SimpleTimer;
#endif
	//string pathToDir = "C:\\Users\\Денис\\Desktop";
	string path1 = pathToDir + "/For data/кімнати.txt";
	string path2 = pathToDir + "/For data/бронь для клієнтів.txt";
	string path3 = pathToDir + "/For data/бронь для організацій.txt";
	string path4 = pathToDir + "/For data/додаткові послуги.txt";
	string path5 = pathToDir + "/For data/організації.txt";
	string path6 = pathToDir + "/For data/ДП клієнти.txt";
	string path7 = pathToDir + "/For data/скарги.txt";
	string path8 = pathToDir + "/For data/витрати.txt";
	InitComplexFromFile(HC, path1);
	InitContracts(HC, path5);
	InitHistoryOfRoom(HC, path2, path3);
	InitExtraService(HC, path4);
	InitExtraServicesClients(HC, path6);
	InitComplaint(HC, path7);
	InitSpendings(HC, path8);
#ifdef _SIMPLE_TIMER_H
	delete timeForReadingFiles;
#endif
	std::time_t t = std::time(0);   // отримати поточну дату
	std::tm* now = std::localtime(&t);
	Date currentDate = {};
	currentDate[2] = now->tm_year + 1900;
	currentDate[1] = now->tm_mon + 1;
	currentDate[0] = now->tm_mday;
	int countOfRequest = 16;
	vector<string> _requestVector{
				" Отримати перелік і загальне число фірм, що забронювали місця в обсязі, не менше вказаного, за весь період співпраці, або за деякий період.",
			"Отримати перелік і загальне число постояльців, що заселяли в номери із зазначеними характеристиками за певний період.",
			"Отримати кількість вільних номерів на даний момент.",
			"Отримати відомості про кількість вільних номерів із зазначеними характеристиками. ",
			"Отримати відомості по конкретному вільному номеру: протягом якого часу він буде пустувати і про його характеристики. ",
			"Отримати список зайнятих зараз номерів, які звільняються до зазначеного терміну. ",
			"Отримати дані про обсяг бронювання номерів даною фірмою за вказаний період, і якими номерами віддавалися переваги. ",
			"Отримати список незадоволених клієнтів та їхні скарги. ",
			"Отримати дані про рентабельність номерів з певними характеристиками: співвідношення про обсяг продажів номерів до накладних витрат за вказаний період.",
			"Отримати відомості про постояльця із заданого номера: його рахунок готелю за додаткові послуги, які надходили від нього скарги, види додаткових послуг, якими він користувався. ",
			"Отримати відомості про фірми, з якими укладені договори про бронь на зазначений період.",
			"Отримати відомості про постояльців, які більше за всіх відвідують готель по всім корпусам готелів, по певним будівлям. ",
			"Отримати відомості про нових клієнтів за вказаний період. ",
			"Отримати відомості про конкретну людину, скільки разів він відвідував готель, в яких номерах і в який період зупинявся, які рахунки оплачував. ",
			"Отримати відомості про конкретний номер: ким він був зайнятий в певний період. ",
			"Отримати відсоткове відношення всіх номерів до номерів, заброньованих партнерами." };
	for (auto& str : _requestVector)
	{
		ChangeItoI(str);
	}

	if (!ErrorAppeared)// якщо інформація считалась правильно, то продовжуємо виконання програми
	{

		bool Executed = false;
		std::cout << "Щоб вивести iнформацiю про певний запит, введiть номер цього запиту." << endl
			<< "Щоб вивести всi запити напишiть 'all'." << endl
			<< "Щоб завершити виконання програми напишiть 'exit'." << endl;
		while (true)
		{
			string str;
			std::cout << "Введи номер запиту, або команду: ";
			std::getline(std::cin, str);
			if (str != "")
			{
				if (str == "exit")
				{
					break;
				}
				else
				{
					if (str == "all")
					{
						for (size_t it = 1; it <= 16; it++)
							std::cout << it << ". " << _requestVector[it - 1] << endl << endl;

						std::cout << endl << endl;
					}
					else
					{
						if (IsStringNumber(str))
						{
							if (1 <= stoi(str) && stoi(str) <= 16)//16 бо всього 16 запитів
							{
								Execute(_requestVector, stoi(str), currentDate);
								Executed = true;
							}
							else
							{
								std::cout << "Запиту з таким номером не має!" << endl;
							}
						}
						else
						{
							if (!Executed)
							{
								std::cout << "Запиту з таким номером не має!" << endl;
								//cin.ignore(3555, '\n');
							}
						}
					}
			}
				}
			Executed = false;
			str.clear();
		}
		DeleteHotelComplex(HC);// очистка пам'яті  
	}
	else
		system("pause");

	//SetConsoleCP(866);
	return 0;
}
//1. Отримати перелік і 
// загальне число фірм, що забронювали місця в обсязі,
//  не менше вказаного, за весь період співпраці, або за деякий період.

//2. Отримати перелік і загальне число постояльців, що заселяли в номери
//  із зазначеними характеристиками за певний період.
// прізвище клієнта, якщо не має то "-1"
//ім'я, якщо не має то "-1"
// кількість відвідувань за певний період, якщо не має то "-1"

//3. Отримати кількість вільних номерів на даний момент.
//

//4. Отримати відомості про кількість вільних номерів із зазначеними характеристиками.(на даний момент) 
// (зірковість корпусу ,місцевість, номер корпусу)

//5. Отримати відомості по конкретному вільному номеру : протягом якого часу він буде 
// пустувати і про його характеристики.(поверх на якому знаходиться, місцевість, ціна)

//6. Отримати список зайнятих зараз номерів, які звільняються до зазначеного терміну.
//

//7. Отримати дані про обсяг бронювання номерів даною фірмою за вказаний період, і якими 
// номерами віддавалися переваги. (обсяг бронювань, які вони не відмовили)

//8. Отримати список незадоволених клієнтів та їхні скарги.
//

//9. Отримати дані про рентабельність номерів з певними характеристиками : співвідношення
//  про обсяг продажів номерів до накладних витрат за вказаний період.

//10. Отримати відомості про постояльця із заданого номера : його рахунок готелю за додаткові 
// послуги, які надходили від нього скарги, види додаткових послуг, якими він користувався.

//11. Отримати відомості про фірми, з якими укладені договори про бронь на зазначений період.
//(і не є скасовані)

//12. Отримати відомості про постояльців, які більше за всіх відвідують готель по всім корпусам 
// готелів, по певним будівлям.(відвідували - не скасували)

//13. Отримати відомості про нових клієнтів за вказаний період.
// клієнт - той хто забронював колись кімнату і не скасував бронь
// відомості - імя, прізвище, перша невідмінена броня


//14. Отримати відомості про конкретну людину, скільки разів він відвідував готель,
//  в яких номерах і в який період зупинявся, які рахунки оплачував.

//15. Отримати відомості про конкретний номер : ким він був зайнятий в певний період.
//

//16. Отримати відсоткове відношення всіх номерів до номерів, заброньованих партнерами.
//`





