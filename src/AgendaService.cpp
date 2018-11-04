#include"Date.hpp"
#include"Meeting.hpp"
#include"Storage.hpp"
#include"AgendaService.hpp"
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    auto filter = [userName, password](const User &user) {
        return (user.getName() == userName && user.getPassword() == password);
    };
    return m_storage->queryUser(filter).size() > 0;
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password, const std::string &email, const std::string &phone) {
    auto filter = [userName, password, email, phone](const User &user) {
        return (user.getName() == userName || user.getEmail() == email || user.getPhone() == phone);
    };
    if(m_storage->queryUser(filter).size() == 0) {
        m_storage->createUser(User(userName, password, email, phone));
        return 1;
    }
    return 0;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    auto filter = [userName, password](const User &user) {
        return (user.getName() == userName && user.getPassword() == password);
    };
    if(m_storage->deleteUser(filter) > 0) {
        deleteAllMeetings(userName);
        return 1;
    }
    return 0;
}

std::list<User> AgendaService::listAllUsers(void) const {
    auto filter = [](const User &user) {
        return 1;
    };
    return m_storage->queryUser(filter);
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title, const std::string &startDate, const std::string &endDate, const std::vector<std::string> &participator) {
    for(int i = 0; i < participator.size(); i ++)
        if(userName == participator[i])
            return 0;
    if(!(Date::isValid(Date::stringToDate(startDate)) && Date::isValid(Date::stringToDate(startDate))))
        return 0;
    auto filter1 = [participator](const User &user) {
        for(int i = 0; i < participator.size(); i ++){
            if(user.getName() == participator[i]){
                return 1;
            }
        }
        return 0;
    };
    if(m_storage->queryUser(filter1).size() != participator.size())
        return 0;
    auto filter2 = [userName](const User &user) {
      return (user.getName() == userName);
    };
    if(!(m_storage->queryUser(filter2).size() > 0))
        return 0;
    auto filter3 = [title](const Meeting &meeting) {
      return (meeting.getTitle() == title);
    };
    if(m_storage->queryMeeting(filter3).size() > 0)
        return 0;
    list<Meeting> l1 = meetingQuery(userName, startDate, endDate);
    auto iter1 = l1.begin();
    for(iter1; iter1 != l1.end(); iter1 ++){
            if((*iter1).getStartDate() > Date::stringToDate(startDate) && (*iter1).getStartDate() < Date::stringToDate(endDate))
                return 0;
            if((*iter1).getEndDate() > Date::stringToDate(startDate) && (*iter1).getEndDate() < Date::stringToDate(endDate))
                return 0;
            if((*iter1).getStartDate() <= Date::stringToDate(startDate) && (*iter1).getEndDate() >= Date::stringToDate(endDate))
                return 0;
    }
    for(int i = 0; i < participator.size(); i ++){
        list<Meeting> l2 = meetingQuery(participator[i], startDate, endDate);
        auto iter2 = l2.begin();
        for(iter2; iter2 != l2.end(); iter2 ++){
            if((*iter2).getStartDate() > Date::stringToDate(startDate) && (*iter2).getStartDate() < Date::stringToDate(endDate))
                return 0;
            if((*iter2).getEndDate() > Date::stringToDate(startDate) && (*iter2).getEndDate() < Date::stringToDate(endDate))
                return 0;
            if((*iter2).getStartDate() <= Date::stringToDate(startDate) && (*iter2).getEndDate() >= Date::stringToDate(endDate))
                return 0;
        }
    }
    m_storage->createMeeting(Meeting(userName, participator, Date::stringToDate(startDate), Date::stringToDate(endDate), title));
    return 1;
}

bool AgendaService::addMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    auto filter = [userName, title, participator](const Meeting &meeting) {
        if(meeting.getSponsor() == userName && meeting.getTitle() == title){
            for(int i = 0; i < meeting.getParticipator().size(); i ++) {
                if(meeting.getParticipator()[i] == participator)
                    return 0;
            }
            return 1;
        }
        return 0;
    };
    auto switcher = [participator](Meeting &meeting){
        meeting.addParticipator(participator);
    };
    return (m_storage->updateMeeting(filter, switcher) > 0);
}

bool AgendaService::removeMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    auto filter = [userName, title, participator](const Meeting &meeting) {
        if(meeting.getSponsor() == userName && meeting.getTitle() == title){
            for(int i = 0; i < meeting.getParticipator().size(); i ++) {
                if(meeting.getParticipator()[i] == participator)
                    return 1;
            }
        }
        return 0;
    };
    auto switcher = [participator](Meeting &meeting){
        meeting.removeParticipator(participator);
    };
    return (m_storage->updateMeeting(filter, switcher) > 0);
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    auto filter = [userName, title](const Meeting &meeting) {
        if(meeting.getTitle() == title){
            if(meeting.getSponsor() == userName)
                return 0;
            for(int i = 0; i < meeting.getParticipator().size(); i ++) {
                if(meeting.getParticipator()[i] == userName)
                    return 1;
            }
        }
        return 0;
    };
    auto switcher = [userName](Meeting &meeting) {
        meeting.removeParticipator(userName);
    };
    return (m_storage->updateMeeting(filter, switcher) > 0);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &title) const {
    auto filter = [userName, title](const Meeting &meeting) {
        if(meeting.getSponsor() == userName && meeting.getTitle() == title)
            return 1;
        for(int i = 0; i < meeting.getParticipator().size(); i ++)
            if(meeting.getParticipator()[i] == userName && meeting.getTitle() == title)
                return 1;
        return 0;
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &startDate, const std::string &endDate) const {
    auto filter = [userName, startDate, endDate](const Meeting &meeting) {
        if(meeting.getSponsor() == userName) {
            if(meeting.getStartDate() >= Date::stringToDate(startDate) && meeting.getStartDate() <= Date::stringToDate(endDate))
                return 1;
            if(meeting.getEndDate() >= Date::stringToDate(startDate) && meeting.getEndDate() <= Date::stringToDate(endDate))
                return 1;
            if(meeting.getStartDate() <= Date::stringToDate(startDate) && meeting.getEndDate() >= Date::stringToDate(endDate))
                return 1;
        }
        for(int i = 0; i < meeting.getParticipator().size(); i ++){
            if(meeting.getParticipator()[i] == userName) {
                if(meeting.getStartDate() >= Date::stringToDate(startDate) && meeting.getStartDate() <= Date::stringToDate(endDate))
                    return 1;
                if(meeting.getEndDate() >= Date::stringToDate(startDate) && meeting.getEndDate() <= Date::stringToDate(endDate))
                    return 1;
                if(meeting.getStartDate() <= Date::stringToDate(startDate) && meeting.getEndDate() >= Date::stringToDate(endDate))
                    return 1;
            }
        }
        return 0;
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    auto filter = [userName](const Meeting &meeting) {
        if(meeting.getSponsor() == userName)
            return 1;
        for(int i = 0; i < meeting.getParticipator().size(); i ++)
            if(meeting.getParticipator()[i] == userName)
                return 1;
        return 0;
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    auto filter = [userName](const Meeting &meeting) {
        return (meeting.getSponsor() == userName);
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
    auto filter = [userName](const Meeting &meeting) {
        for(int i = 0; i < meeting.getParticipator().size(); i ++)
            if(meeting.getParticipator()[i] == userName)
                return 1;
        return 0;
    };
    return m_storage->queryMeeting(filter);
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    auto filter = [userName, title](const Meeting &meeting) {
        return (meeting.getSponsor() == userName && meeting.getTitle() == title);
    };
    return (m_storage->deleteMeeting(filter) > 0);
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
    auto filter = [userName](const Meeting &meeting) {
        return (meeting.getSponsor() == userName);
    };
    return (m_storage->deleteMeeting(filter) > 0);
}

void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    m_storage->sync();
}
