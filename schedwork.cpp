#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& schedule, vector<int>& shiftsAssigned, size_t currentDay, size_t currentSlot);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // get total number of days and workers from availability matrix
    size_t totalDays = avail.size();
    size_t totalWorkers = avail[0].size();

    // initialize schedule with empty daily vectors
    // each day stores list of worker ids assigned
    for(size_t day = 0; day < totalDays; day++) {
      sched.push_back(vector<Worker_T>());
    }
    // initialize shift tracker for each worker
    // vector keeps track of total shifts each worker has worked
    vector<int> shiftsAssigned(totalWorkers, 0);

    // begin with day 0 and slot 0 for recursive backtracking
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsAssigned, 0, 0);
}

bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& schedule, vector<int>& shiftsAssigned, size_t currentDay, size_t currentSlot) {
  size_t totalDays = avail.size();
  size_t totalWorkers = avail[0].size();

  if (currentDay == totalDays) { // base case if we have filled all days schedule is complete and valid
    return true;
  }
  // if curr day fully scheduled move to next day and start at slot 0
  if (currentSlot == dailyNeed) {
    return scheduleHelper(avail, dailyNeed, maxShifts, schedule, shiftsAssigned, currentDay + 1, 0);
  }
  // try assigning each worker to curr slot
  for (size_t workerID =0; workerID < totalWorkers; workerID++) {
    bool isAvailable = avail[currentDay][workerID]; 
    bool underShiftLimit = shiftsAssigned[workerID] < static_cast<int>(maxShifts); 
    bool notAlreadyScheduled = find(schedule[currentDay].begin(), schedule[currentDay].end(), workerID) == schedule[currentDay].end();
    
    // if all conditions are met assign worker to curr slot
    if (isAvailable && underShiftLimit && notAlreadyScheduled) {
      schedule[currentDay].push_back(workerID);
      shiftsAssigned[workerID]++; 

      // recursively try to fill next slot
      if(scheduleHelper(avail, dailyNeed, maxShifts, schedule, shiftsAssigned, currentDay, currentSlot +1)) {
        return true; // if valid schedule
      }

      schedule[currentDay].pop_back(); // undo assignment if recursion fails
      shiftsAssigned[workerID]--;
    }
  }
  return false; // if no valid worker can be assigned to slot return false
}

