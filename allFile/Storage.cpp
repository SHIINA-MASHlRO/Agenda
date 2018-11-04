#include"Path.hpp"
#include"Meeting.hpp"
#include"Storage.hpp"
#include<functional>
#include<string>
#include<iostream>
#include<list>
#include<fstream>
#include<vector>
using namespace std;
shared_ptr<Storage> Storage::m_instance = nullptr;
Storage::Storage(){
  m_dirty = false;
  readFromFile();
}
bool Storage::readFromFile(void) {
  ifstream meetingFile(Path::meetingPath);
  ifstream userFile(Path::userPath);
  if(meetingFile.is_open() == false || userFile.is_open() == false) {
   return false;
  }
  while(true) {
    vector<int> sign;
  	string str;
  	getline(userFile, str);
  	for(int i = 0; i < str.size() ; i++) {
    	if(str[i] == '"')
    	sign.push_back(i);
  	}
  	if(sign.size() != 8)
    	break;
   	string t_name;
   	string t_password;
   	string t_email;
   	string t_phone;
   	for(int i = sign[0]+1; i < sign[1]; i++)
    	t_name.push_back(str[i]);
   	for(int i = sign[2]+1; i < sign[3]; i++)
    	t_password.push_back(str[i]);
   	for(int i = sign[4]+1; i < sign[5]; i++)
    	t_email.push_back(str[i]);
   	for(int i = sign[6]+1; i < sign[7]; i++)
    	t_phone.push_back(str[i]);
    User t_user(t_name, t_password, t_email, t_phone);
  		m_userList.push_back(t_user);
  }
  userFile.close();
  while(true){
    vector<int> sign1;
    vector<int> sign2;
    string str;
    getline(meetingFile, str);
    for(int i = 0; i < str.size(); i++) {
      if(str[i] == '"')
        sign1.push_back(i);
      if(str[i] == '&')
        sign2.push_back(i);
    }
    if(sign1.size() != 10)
      break;
    string t_sponsor;
    vector<string> t_participator;
    string t_startDate;
    string t_endDate;
    string t_title;
    for(int i = sign1[0]+1; i < sign1[1]; i++)
      t_sponsor.push_back(str[i]);
    for(int i = sign1[4]+1; i < sign1[5]; i++)
      t_startDate.push_back(str[i]);
    for(int i = sign1[6]+1; i < sign1[7]; i++)
      t_endDate.push_back(str[i]);
    for(int i = sign1[8]+1; i < sign1[9]; i++)
      t_title.push_back(str[i]);
    if(sign2.size() != 0){
    	for(int i = 0 ; i <= sign2.size() ; i++) {
      	if(i == 0){
        	string temp;
        	for(int j = sign1[2]+1 ; j < sign2[i]; j++)
              temp.push_back(str[j]);
        	t_participator.push_back(temp);
        	continue;
      	}
      	if(i == sign2.size()){
        	string temp;
        	for(int j = sign2[i-1]+1 ; j < sign1[3] ; j++)
              temp.push_back(str[j]);
        	t_participator.push_back(temp);
        	continue;
      	}
      	string temp;
      	for(int j = sign2[i-1]+1 ; j < sign2[i] ; j++)
        	temp.push_back(str[j]);
      	t_participator.push_back(temp);
      }
    }
    else{
      string temp;
      for(int i = sign1[2]+1; i < sign1[3]; i++)
        temp.push_back(str[i]);
    	t_participator.push_back(temp);
    }
    Date s(t_startDate);
    Date e(t_endDate);
    Meeting t_meeting(t_sponsor, t_participator, s, e, t_title);
    m_meetingList.push_back(t_meeting);
  }
  meetingFile.close();
  return true;
}

bool Storage::writeToFile(void) {
  m_dirty = false;
  ofstream userFile(Path::userPath, ios::out);
  ofstream meetingFile(Path::meetingPath, ios::out);
  if(meetingFile.is_open() == false || userFile.is_open() == false) {
   return false;
  }
  if(m_userList.size() == 0 && m_meetingList.size() == 0)
    return false;
  auto it = m_userList.begin();
  for(it; it != m_userList.end(); it++) {
    userFile << "\"" <<(*it).getName() << "\"" << ','<< "\"" << (*it).getPassword() << "\"" << ',';
    userFile << "\"" << (*it).getEmail() << "\"" <<',' << "\"" << (*it).getPhone() << "\"" << "\n";
  }
  userFile.close();
  auto ft = m_meetingList.begin();
  for(ft; ft != m_meetingList.end(); ft++) {
    meetingFile<<'"'<<(*ft).getSponsor()<<'"'<<','<<'"';
    vector<string> t_participator = (*ft).getParticipator();
    for(int i = 0 ; i < t_participator.size() ; i++) {
      meetingFile << t_participator[i];
      if(i != t_participator.size() - 1)
  		meetingFile << '&';
    }
    meetingFile<< '"' << ',';
    meetingFile << '"' << (*ft).getStartDate().dateToString((*ft).getStartDate()) << '"' << ',';
    meetingFile<< '"' << (*ft).getEndDate().dateToString((*ft).getEndDate()) << '"' << ',' << '"'<< (*ft).getTitle() << '"' << "\n";
  }
    meetingFile.close();
    return true;
}
shared_ptr<Storage> Storage::getInstance(void) {
  if(m_instance == nullptr)
    m_instance =  shared_ptr<Storage> (new Storage());
  return m_instance;
}
Storage::~Storage(){
  sync();
  Storage::m_instance = nullptr;
}

void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
  m_dirty = true;
}
list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
  list<User> t_userList;
  auto iter = m_userList.begin();
  for(iter; iter!= m_userList.end(); iter++) {
    if(filter(*iter))
    	t_userList.push_back(*iter);
  }
  return t_userList;
}
int Storage::updateUser(std::function<bool(const User &)> filter, std::function<void(User &)> switcher) {
  int count = 0;
 	auto iter = m_userList.begin();
  for(iter; iter != m_userList.end(); iter++) {
    if(filter(*iter)) {
    	switcher(*iter);
  		count++;
    }
	}
  if(count != 0)
  	m_dirty = true;
  return count;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
  auto iter = m_userList.begin();
  int count = 0;
  for(iter; iter != m_userList.end(); ) {
    if(filter(*iter)){
    	auto it = iter;
     	iter = m_userList.erase(it);
     	count++;
     	continue;
    }
    iter++;
  }
  if(count != 0)
  	m_dirty = true;
  return count;
}

void Storage::createMeeting(const Meeting& t_meeting) {
  m_meetingList.push_back(t_meeting);
  m_dirty=true;
}

list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
  list<Meeting> t_meetingList;
  auto iter = m_meetingList.begin();
  for(iter; iter != m_meetingList.end(); iter++) {
    if(filter(*iter))
    	t_meetingList.push_back(*iter);
  }
  return t_meetingList;
}
int Storage::updateMeeting(std::function<bool (const Meeting &)> filter, std::function<void(Meeting &)> switcher) {
  int count = 0;
  auto iter = m_meetingList.begin();
  for(iter; iter != m_meetingList.end(); iter++) {
      if(filter(*iter)) {
        switcher(*iter);
 				count++;
  		}
  }
  if(count != 0)
  	m_dirty = true;
  return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
  int count = 0;
  auto iter = m_meetingList.begin();
  for(iter; iter!= m_meetingList.end(); ) {
    if(filter(*iter)) {
     	count++;
      auto it = iter;
      iter = m_meetingList.erase(it);
      continue;
    }
    iter++;
  }
  if(count != 0)
  m_dirty = true;
  return count;
}
bool Storage::sync() {
  if(m_dirty == false)
    return false;
  if(m_dirty == true) {
    if(writeToFile() == false)
      return false;
  }
  return true;
}
