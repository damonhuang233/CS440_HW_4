#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <cstring>

using namespace std;

#define MM_SIZE 22

struct EmpBlock {
    int eid;
    string ename;
    int age;
    double salary;
};

bool cmp_emp( EmpBlock e1, EmpBlock e2 );
bool grabEmp(ifstream &empin, EmpBlock &emp);
bool grabEmpEid(ifstream &empin, int &eid);
int create_runs_emp();
bool next_emp(int n_emp_runs, ifstream empruns[], EmpBlock &emp);
int merge_runs(int p, int runs);
void write_to_runs(EmpBlock emp, ofstream &out);

int merge_size = MM_SIZE - 1;

int
main()
{
  int n_p0_runs = create_runs_emp();

  int p = 0;
  int runs = n_p0_runs;

  while(runs > 1)
  {
    p++;
    runs = merge_runs(p, runs);
  }

  string oldfilename = "p";
  oldfilename += to_string(p);
  oldfilename += "_0.run";

  string newfilename = "EmpSorted.csv";

  int result = rename(oldfilename.c_str(), newfilename.c_str());
  if ( result != 0 )
    cout << "Can not rename file" << oldfilename << " to " << newfilename << endl;

  return 0;
}

bool
cmp_emp( EmpBlock e1, EmpBlock e2 )
{
  return e1.eid < e2.eid;
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
      //cout << "file " << i << " : " << eid << endl;
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

    string filename = "p0_";
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

void
write_to_runs(EmpBlock emp, ofstream &fout)
{
  fout << emp.eid << ',' << emp.ename << ',' << emp.age
    << ',' << emp.salary << ',' << "\n";
}

int
merge_runs(int p, int runs)
{
  //cout << "p: " << p << endl;

  int res_runs = runs / merge_size;
  if ((runs % merge_size) > 0)
    res_runs++;

  ofstream outruns[res_runs];
  for (int i = 0; i < res_runs; i++)
  {
    int start = i * merge_size;
    int end = start + merge_size - 1;
    if (end > runs - 1)
      end = runs - 1;
    int n_runs = end - start + 1;

    //cout << start << " - " << end << " : " << n_runs << endl;

    ifstream inruns[n_runs];
    for (int j = 0; j < n_runs; j++)
    {
      string filename = "p";
      filename += to_string(p - 1);
      filename += "_";
      filename += to_string(j + start);
      filename += ".run";
      inruns[j].open(filename, ios::in);
    }

    string outfilename = "p";
    outfilename += to_string(p);
    outfilename += "_";
    outfilename += to_string(i);
    outfilename += ".run";
    outruns[i].open(outfilename, ios::out | ios::trunc);

    EmpBlock output_buffer;
    while(next_emp(n_runs, inruns, output_buffer))
      write_to_runs(output_buffer, outruns[i]);

    for (int j = 0; j < n_runs; j++)
      inruns[j].close();
    outruns[i].close();
  }


  return res_runs;
}
