#include "Date.hpp"
#include <string>
using namespace std;
Date::Date() {
  m_year = m_month = m_day = m_hour = m_minute = 0;
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
  m_year = t_year;
  m_month = t_month;
  m_day = t_day;
  m_hour = t_hour;
  m_minute = t_minute;
}
Date::Date(const std::string &dateString) {
  int y, mth, d, h, m;
	y = mth = d = h = m = 0;
	if(dateString.size() != 16){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
	if( dateString[4] != '-' || dateString[7] != '-'|| dateString[10] != '/' || dateString[13] != ':' ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
	if( dateString[0] < '1' || dateString[0] > '9' ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
	for( int i = 0; i < 4; i ++ ) {
		int temp = dateString[i] - '0';
		if( temp < 0 || temp > 9 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
        }
		y = y * 10 + temp;
	}
	if( dateString[5] < '0' || dateString[5] > '1' ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
        }
	for( int i = 0; i < 2; i ++ ) {
		int temp = dateString[5 + i] - '0';
		if( temp < 0 || temp > 9){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
        }
		mth = mth * 10 + temp;
	}
	if( mth < 1 || mth > 12 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
    if( dateString[8] < '0' || dateString[8] > '3' ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
    }
  for( int i = 0; i < 2; i ++ ) {
    int temp = dateString[8 + i] - '0';
    if( temp < 0 || temp > 9){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
    d = d * 10 + temp;
  }
  if( d < 1 || d > 31 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
  if( mth == 2){
    if(d > 29){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
    if( !( ( y % 4 == 0 && y % 100 != 0) || (y % 400 == 0) )&& d > 28){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
  }
  if( ( mth == 2 || mth == 4 || mth == 6 || mth == 9 || mth == 11 ) && d > 30 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
  if( dateString[11] < '0' || dateString[11] > '2' ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
  }
  for(int i = 0; i < 2; i ++ ) {
    int temp = dateString[11 + i] - '0';
    if( temp < 0 || temp > 9 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
    h = h * 10 + temp;
  }
  if( h < 0 || h > 23 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
  if( dateString[14] < '0' || dateString[14] > '6' ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
  for( int i = 0; i < 2; i ++ ) {
    int temp = dateString[14 + i] - '0';
    if( temp < 0 || temp > 9 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
    m = m * 10 + temp;
  }
  if( m < 0 || m > 59 ){
        m_year = m_month = m_day = m_hour = m_minute = 0;
        return;
	}
    m_year = y;
    m_month = mth;
    m_day = d;
    m_hour = h;
    m_minute = m;
}
int Date::getYear(void) const {
  return m_year;
}
void Date::setYear(const int t_year) {
  m_year = t_year;
}
int Date::getMonth(void) const {
  return m_month;
}
void Date::setMonth(const int t_month) {
  m_month = t_month;
}
int Date::getDay(void) const {
  return m_day;
}
void Date::setDay(const int t_day) {
  m_day = t_day;
}
int Date::getHour(void) const {
  return m_hour;
}
void Date::setHour(const int t_hour) {
  m_hour = t_hour;
}
int Date::getMinute(void) const {
  return m_minute;
}
void Date::setMinute(const int t_minute) {
  m_minute = t_minute;
}
 bool Date::isValid(const Date &t_date) {
  if( t_date.getYear() < 1000 || t_date.getYear() > 9999 )
    return 0;
  if( t_date.getMonth() < 1 || t_date.getMonth() > 12 )
    return 0;
  if( t_date.getDay() < 1 || t_date.getDay() > 31 )
    return 0;
  if( t_date.getMonth() == 2){
    if(t_date.getDay() > 29)
        return 0;
    if( !( ( t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0) || (t_date.getYear() % 400 == 0) ) && t_date.getDay() > 28)
        return 0;
  }
  if( ( t_date.getMonth() == 2 || t_date.getMonth() == 4 || t_date.getMonth() == 6 || t_date.getMonth() == 9 || t_date.getMonth() == 11 ) && t_date.getDay() > 30)
    return 0;
  if( t_date.getHour() < 0 || t_date.getHour() > 23 )
    return 0;
  if( t_date.getMinute() < 0 || t_date.getMinute() > 59 )
    return 0;
  return 1;
}
Date Date::stringToDate(const std::string &t_dateString) {
  int y, mth, d, h, m;
	y = mth = d = h = m = 0;
	if(t_dateString.size() != 16)
        return Date(0, 0, 0, 0, 0);
	if( t_dateString[4] != '-' || t_dateString[7] != '-' || t_dateString[10] != '/' || t_dateString[13] != ':' )
		return Date( 0, 0, 0, 0, 0 );
	if( t_dateString[0] < '1' || t_dateString[0] > '9' )
		return Date( 0, 0, 0, 0, 0 );
	for( int i = 0; i < 4; i ++ ) {
		int temp = t_dateString[i] - '0';
		if( temp < 0 || temp > 9 )
			return Date( 0, 0, 0, 0, 0 );
		y = y * 10 + temp;
	}
	if( t_dateString[5] < '0' || t_dateString[5] > '1' )
		return Date( 0, 0, 0, 0, 0 );
	for( int i = 0; i < 2; i ++ ) {
		int temp = t_dateString[5 + i] - '0';
		if( temp < 0 || temp > 9)
			return Date( 0, 0, 0, 0, 0 );
		mth = mth * 10 + temp;
	}
	if( mth < 1 || mth > 12 )
		return Date( 0, 0, 0, 0, 0 );
  if( t_dateString[8] < '0' || t_dateString[8] > '3' )
    return Date( 0, 0, 0, 0, 0 );
  for( int i = 0; i < 2; i ++ ) {
    int temp = t_dateString[8 + i] - '0';
    if( temp < 0 || temp > 9)
			return Date( 0, 0, 0, 0, 0 );
    d = d * 10 + temp;
  }
  if( d < 1 || d > 31 )
		return Date( 0, 0, 0, 0, 0 );
  if( mth == 2 ){
    if(d > 29){
        return  Date(0, 0, 0, 0, 0);
	}
    if( !( ( y % 4 == 0 && y % 100 != 0) || (y % 400 == 0) )&& d > 28){
        return  Date(0, 0, 0, 0, 0);
    }
  }
  if( ( mth == 2 || mth == 4 || mth == 6 || mth == 9 || mth == 11 ) && d > 30 )
    return Date( 0, 0, 0, 0, 0 );
  if( t_dateString[11] < '0' || t_dateString[11] > '2' )
    return Date( 0, 0, 0, 0, 0 );
  for(int i = 0; i < 2; i ++ ) {
    int temp = t_dateString[11 + i] - '0';
    if( temp < 0 || temp > 9 )
      	return Date( 0, 0, 0, 0, 0 );
    h = h * 10 + temp;
  }
  if( h < 0 || h > 23 )
    return Date( 0, 0, 0, 0, 0 );
  if( t_dateString[14] < '0' || t_dateString[14] > '6' )
    return Date( 0, 0, 0, 0, 0 );
  for( int i = 0; i < 2; i ++ ) {
    int temp = t_dateString[14 + i] - '0';
    if( temp < 0 || temp > 9 )
      return Date( 0, 0, 0, 0, 0 );
    m = m * 10 + temp;
  }
  if( m < 0 || m > 59 )
  	return Date( 0, 0, 0, 0, 0 );
 	return Date( y, mth, d, h, m );
}
std::string Date::dateToString(const Date &t_date) {
  string s;
  if(isValid(t_date) == true){
  	s += (t_date.getYear() / 1000 + '0');
    s += ((t_date.getYear() / 100) % 10 + '0');
    s += ((t_date.getYear() / 10) % 10 + '0');
    s += (t_date.getYear() % 10 + '0');
    s += '-';
    s += (t_date.getMonth() / 10 + '0');
    s += (t_date.getMonth() % 10 + '0');
    s += '-';
    s += (t_date.getDay() / 10 + '0');
    s += (t_date.getDay() % 10 + '0');
    s += '/';
    s += (t_date.getHour() / 10 + '0');
    s += (t_date.getHour() % 10 + '0');
    s += ':';
    s += (t_date.getMinute() / 10 + '0');
    s += (t_date.getMinute() % 10 + '0');
  }
 	else{
    s = "0000-00-00/00:00";
  }
  return s;
}
Date &Date::operator=(const Date &t_date) {
  m_year = t_date.getYear();
  m_month = t_date.getMonth();
  m_day = t_date.getDay();
  m_hour = t_date.getHour();
  m_minute = t_date.getMinute();
}
bool Date::operator==(const Date &t_date) const {
  if(m_year == t_date.getYear() && m_month == t_date.getMonth() && m_day == t_date.getDay() && m_hour == t_date.getHour() && m_minute == t_date.getMinute())
    return 1;
  return 0;
}
bool Date::operator>(const Date &t_date) const {
  string s1 = dateToString(*this);
  string s2 = dateToString(t_date);
  if(s1 == s2)
    return 0;
  for(int i = 0; i < s1.size(); i ++){
        if(s1[i] != s2[i]){
            if(s1[i] < s2[i])
                return 0;
            return 1;
        }
  }
    return 1;
}
bool Date::operator<(const Date &t_date) const {
  string s1 = dateToString(*this);
  string s2 = dateToString(t_date);
  if(s1 == s2)
    return 0;
  for(int i = 0; i < s1.size(); i ++){
        if(s1[i] != s2[i]){
            if(s1[i] > s2[i])
                return 0;
            return 1;
        }
  }
    return 1;
}
bool Date::operator>=(const Date &t_date) const {
  string s1 = dateToString(*this);
  string s2 = dateToString(t_date);
  if(s1 == s2)
    return 1;
  for(int i = 0; i < s1.size(); i ++){
        if(s1[i] != s2[i]){
            if(s1[i] < s2[i])
                return 0;
            return 1;
        }
  }
    return 1;
}
bool Date::operator<=(const Date &t_date) const {
  string s1 = dateToString(*this);
  string s2 = dateToString(t_date);
  if(s1 == s2)
    return 1;
  for(int i = 0; i < s1.size(); i ++){
        if(s1[i] != s2[i]){
            if(s1[i] > s2[i])
                return 0;
            return 1;
        }
  }
    return 1;
}





