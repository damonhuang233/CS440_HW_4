#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

#define MM_SIZE 22

struct EmpBlock {
    int eid;
    string ename;
    int age;
    double salary;
};

struct DeptBlock {
    int did;
    string dname;
    double budget;
    int managerid;
};

int create_runs_dept();
int create_runs_emp();
bool cmp_dept( DeptBlock d1, DeptBlock d2 );
bool cmp_emp( EmpBlock e1, EmpBlock e2 );
bool grabDept(ifstream &deptin, DeptBlock &dept);
bool grabDeptEid(ifstream &deptin, int &manid);
bool grabEmp(ifstream &empin, EmpBlock &emp);
bool grabEmpEid(ifstream &empin, int &eid);
void merge_join(int n_emp_runs, int n_dept_runs);
bool next_emp(int n_emp_runs, ifstream empruns[], EmpBlock &emp);
bool next_dept(int n_dept_runs, ifstream deptruns[], DeptBlock &dept);
int to_real_idx(int start, int idx);

int
main()
{
  int n_emp_runs = create_runs_emp();
  int n_dept_runs = create_runs_dept();
  merge_join(n_emp_runs, n_dept_runs);
  return 0;
}

int
to_real_idx(int start, int idx)
{
  return (start + idx)%(MM_SIZE / 2);
}

int
create_runs_dept()
{
  ifstream deptin;
  ofstream dept_run_out;
  deptin.open("Dept.csv", ios::in);
  int n_runs = 0;
  DeptBlock deptBuffer[MM_SIZE];

  while(1)
  {
    int i = 0;
    for ( i = 0; i < MM_SIZE; i++ )
    {
      bool res = grabDept(deptin, deptBuffer[i]);
      if ( !res )
        break;
    }

    sort(deptBuffer, deptBuffer + i, cmp_dept);

    string filename = "d";
    filename += to_string(n_runs);
    filename += ".run";

    dept_run_out.open(filename, ios::out | ios::trunc);

    for (int j = 0; j < i; j++)
    {
      string data = "";
      data += to_string(deptBuffer[j].did);
      data += ",";
      data += deptBuffer[j].dname;
      data += ",";
      data += to_string(deptBuffer[j].budget);
      data += ",";
      data += to_string(deptBuffer[j].managerid);
      data += "\n";
      dept_run_out << data;
    }

    dept_run_out.close();

    n_runs++;

    if ( i < MM_SIZE )
      break;
  }

  deptin.close();

  return n_runs;
}

int
create_runs_emp()
{
  ifstream empin;
  ofstream emp_run_out;
  empin.open("Emp.csv", ios::in);
  int n_runs = 0;
  EmpBlock empBuffer[MM_SIZE];

  while(1)
  {
    int i = 0;
    for ( i = 0; i < MM_SIZE; i++ )
    {
      bool res = grabEmp(empin, empBuffer[i]);
      if ( !res )
        break;
    }

    sort(empBuffer, empBuffer + i, cmp_emp);

    string filename = "e";
    filename += to_string(n_runs);
    filename += ".run";

    emp_run_out.open(filename, ios::out | ios::trunc);

    for (int j = 0; j < i; j++)
    {
      string data = "";
      data += to_string(empBuffer[j].eid);
      data += ",";
      data += empBuffer[j].ename;
      data += ",";
      data += to_string(empBuffer[j].age);
      data += ",";
      data += to_string(empBuffer[j].salary);
      data += "\n";
      emp_run_out << data;
    }

    emp_run_out.close();

    n_runs++;

    if ( i < MM_SIZE )
      break;
  }

  empin.close();

  return n_runs;
}

bool
cmp_dept( DeptBlock d1, DeptBlock d2 )
{
  return d1.managerid < d2.managerid;
}

bool cmp_emp( EmpBlock e1, EmpBlock e2 )
{
  return e1.eid < e2.eid;
}

bool
grabDept(ifstream &deptin, DeptBlock &dept)
{
    string line, word;
    if (getline(deptin, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        dept.did = stoi(word);
        getline(s, word, ',');
        dept.dname = word;
        getline(s, word, ',');
        dept.budget = stod(word);
        getline(s, word, ',');
        dept.managerid = stoi(word);
        return true;
    } else {
        return false;
    }
}

bool
grabEmp(ifstream &empin, EmpBlock &emp)
{
  string line, word;
  if (getline(empin, line, '\n')) {
      stringstream s(line);
      getline(s, word,',');
      emp.eid = stoi(word);
      getline(s, word, ',');
      emp.ename = word;
      getline(s, word, ',');
      emp.age = stoi(word);
      getline(s, word, ',');
      emp.salary = stod(word);
      return true;
  } else {
      return false;
  }
}

bool
grabEmpEid(ifstream &empin, int &eid)
{
  string line, word;
  if (getline(empin, line, '\n')) {
      stringstream s(line);
      getline(s, word,',');
      eid = stoi(word);
      return true;
  } else {
      return false;
  }
}

bool
grabDeptEid(ifstream &deptin, int &manid)
{
  string line, word;
  if (getline(deptin, line, '\n')) {
      stringstream s(line);
      getline(s, word,',');
      getline(s, word,',');
      getline(s, word,',');
      getline(s, word,',');
      manid = stoi(word);
      return true;
  } else {
      return false;
  }
}

void
merge_join(int n_emp_runs, int n_dept_runs)
{
  int e_start = 0;
  int e_size = 0;
  EmpBlock empBuffer[MM_SIZE / 2];

  int d_start = 0;
  int d_size = 0;
  DeptBlock deptBuffer[MM_SIZE / 2];

  ifstream empruns[n_emp_runs];
  for (int i = 0; i < n_emp_runs; i++)
  {
    string filename = "e";
    filename += to_string(i);
    filename += ".run";
    empruns[i].open(filename, ios::in);
  }

  ifstream deptruns[n_dept_runs];
  for (int i = 0; i < n_dept_runs; i++)
  {
    string filename = "d";
    filename += to_string(i);
    filename += ".run";
    deptruns[i].open(filename, ios::in);
  }

  for (int i = 0; i < MM_SIZE / 2; i++)
  {
    if (next_emp(n_emp_runs, empruns, empBuffer[i]))
      e_size++;
    else
      break;
  }

  for (int i = 0; i < MM_SIZE / 2; i++)
  {
    if (next_dept(n_dept_runs, deptruns, deptBuffer[i]))
      d_size++;
    else
      break;
  }

  int mark = -1;
  ofstream join_out;
  join_out.open("debug.csv", ios::out | ios::trunc);
  while(1)
  {
    int e_idx, d_idx, eid, manid;

    if (mark == -1)
    {
      e_idx = to_real_idx(e_start, 0);
      d_idx = to_real_idx(d_start, 0);
      eid = empBuffer[e_idx].eid;
      manid = deptBuffer[d_idx].managerid;

      while (eid < manid)
      {
        int end_idx = to_real_idx(e_start, e_size);
        int res = next_emp(n_emp_runs, empruns, empBuffer[end_idx]);
        if ( !res )
          e_size--;
        e_start = (e_start + 1) % (MM_SIZE / 2);
        e_idx = to_real_idx(e_start, 0);
        eid = empBuffer[e_idx].eid;
      }

      while (eid > manid)
      {
        int end_idx = to_real_idx(d_start, d_size);
        int res = next_dept(n_dept_runs, deptruns, deptBuffer[end_idx]);
        if ( !res )
          d_size--;
        d_start = (d_start + 1) % (MM_SIZE / 2);
        d_idx = to_real_idx(d_start, 0);
        manid = deptBuffer[d_idx].managerid;
      }

      mark = 0;
    }

    if (eid == manid)
    {
      
    }


  }

  join_out.close();

  for (int i = 0; i < n_emp_runs; i++)
    empruns[i].close();

  for (int i = 0; i < n_dept_runs; i++)
    deptruns[i].close();
}

bool
next_emp(int n_emp_runs, ifstream empruns[], EmpBlock &emp)
{
  int min_idx = -1;
  int min_eid = -1;
  int eid = -1;
  for (int i = 0; i < n_emp_runs; i++){
    streampos prev = empruns[i].tellg();
    bool res = grabEmpEid(empruns[i], eid);
    if ( res )
    {
      if ( min_eid == -1 )
      {
        min_eid = eid;
        min_idx = i;
      }
      if ( eid < min_eid )
      {
        min_eid = eid;
        min_idx = i;
      }
      empruns[i].seekg(prev);
    }
  }

  if ( min_idx == -1 )
    return false;
  else
  {
    grabEmp(empruns[min_idx], emp);
    return true;
  }
}

bool
next_dept(int n_dept_runs, ifstream deptruns[], DeptBlock &dept)
{
  int min_idx = -1;
  int min_manid = -1;
  int manid = -1;
  for (int i = 0; i < n_dept_runs; i++){
    streampos prev = deptruns[i].tellg();
    bool res = grabDeptEid(deptruns[i], manid);
    if ( res )
    {
      if ( min_manid == -1 )
      {
        min_manid = manid;
        min_idx = i;
      }
      if ( manid < min_manid )
      {
        min_manid = manid;
        min_idx = i;
      }
      deptruns[i].seekg(prev);
    }
  }

  if ( min_idx == -1 )
    return false;
  else
  {
    grabDept(deptruns[min_idx], dept);
    return true;
  }
}
