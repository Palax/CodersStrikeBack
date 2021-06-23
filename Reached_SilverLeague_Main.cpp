#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct Vec2 {
    Vec2(const int _x, const int _y) 
        : x(_x), y(_y) {}
    float Distance(const Vec2 _other) const {
        return sqrt(pow(_other.x - x, 2) + pow(_other.y - y, 2) * 1.0);
    }
    bool IsEqual(const Vec2& _other) const {
        return _other.x == x && _other.y == y; 
    }
    bool IsNear(const Vec2& _other, const int _nearRange = 600) const {
        return x >= (_other.x - _nearRange) && x <= (_other.x + _nearRange) 
            && y >= (_other.y - _nearRange) && y <= (_other.y + _nearRange);
    }
    void MakePerpendicular(const int _sign)
    {
        int sign = clamp<int>(_sign, -1, 1);
        int tx = x;
        int ty = y;
        x = ty * sign;
        y = tx * (sign * -1);
    }
    float Length()
    {
        return sqrt((x * x) + (y * y));
    }
    Vec2 operator-(const Vec2& _other) const
    {
        return Vec2(x - _other.x, y - _other.y);
    }
    Vec2 operator+(const Vec2& _other) const
    {
        return Vec2(x + _other.x, y + _other.y);
    }
    Vec2 operator*(const float& _other) const
    {
        return Vec2(x * _other, y * _other);
    }     
    int x;
    int y;
    static const Vec2 sZero;
};

const Vec2 Vec2::sZero(0,0);

class GameWorld
{
public:
    GameWorld() 
    : m_FirstLapCpointIndex(-1), m_LargestLapSegmentIndex(-1) {}

    int GetSegmentsNum() const {
        return m_CpointsPositions.size();
    }

    void AddCheckpoint(const Vec2& _pos) {
        m_CpointsPositions.push_back(_pos);
        cerr << "Cpoint[0](" << m_CpointsPositions.begin()->x << " " << m_CpointsPositions.begin()->y << ") is equal to (" << _pos.x << " " << _pos.y << ") = " << (m_CpointsPositions.size() > 1 && m_CpointsPositions.begin()->IsEqual(_pos)) << endl;
        if(m_FirstLapCpointIndex == -1 && m_CpointsPositions.size() > 1 && m_CpointsPositions.begin()->IsNear(_pos)) {
            m_FirstLapCpointIndex = m_CpointsPositions.size() - 1;
        }
    }
    int GetSegmentDistance(const int _lapNr) {
        if(_lapNr >= 0 && _lapNr < m_CpointsPositions.size())
            return m_CpointsPositions[_lapNr].Distance(m_CpointsPositions[_lapNr + 1]);
        else return -1;
    }
    int GetLastSegmentDistance() {
        if(m_CpointsPositions.size() < 2) return -1;
        else return m_CpointsPositions[m_CpointsPositions.size() - 2].Distance(m_CpointsPositions[m_CpointsPositions.size() - 1]);
    }
    bool IsFirstLapCpointFound() {
        return m_FirstLapCpointIndex > 0;
    }
    int GetLapEndingCpointIndex() const
    {
        return m_FirstLapCpointIndex;
    }
    int GetLargestLapSegmentIndex() const{
        return m_LargestLapSegmentIndex;
    }
    const Vec2& GetLastCpoint() const{
        if(m_CpointsPositions.size() == 0) return Vec2::sZero;
        return *m_CpointsPositions.end();
    }

    void OnStartRace(const Vec2& _checkpoint)
    {
        m_CpointsPositions.push_back(_checkpoint);
        cerr << "-====START RACE at (" << _checkpoint.x << " " << _checkpoint.y << ")====-" << endl;
    }
    void OnCheckpointReached(const Vec2& _checkpoint)
    {
        AddCheckpoint(_checkpoint);

        cerr << "-====Checkpoint REACHED: " << (GetSegmentsNum() - 1);
        if(GetSegmentsNum() > 1) {
            cerr << " with segment distance " << GetLastSegmentDistance() << endl;
        } 
        else cerr << "====-" << endl;
    }
    void OnFirstLapCompleted(const Vec2& _checkpoint)
    {
        if(m_CpointsPositions.size() < 2) return;
        float maxDistance = 0.f;
        int largestSegment = 0;
        for(int i = 0; i < m_CpointsPositions.size() - 2; ++i)
        {
            if(maxDistance == 0.f) {
                maxDistance = m_CpointsPositions[i].Distance(m_CpointsPositions[i+1]);
                largestSegment = 0;
            }
            else {
                float fDistance = m_CpointsPositions[i].Distance(m_CpointsPositions[i+1]);
                if(fDistance > maxDistance)
                {
                    maxDistance = fDistance;
                    largestSegment = i;
                }
            }
        }
        m_LargestLapSegmentIndex = largestSegment;
        cerr << "-====Lap completed with " << GetSegmentsNum() - 1 << " checkpoints and largest segment is " << m_LargestLapSegmentIndex << " ====-" << endl;
    }

private:
    vector<Vec2> m_CpointsPositions;
    /*When a lap is completed, this index will represent the last checkpoint index*/
    int          m_FirstLapCpointIndex;
    /*What is the largest distance between two checkpoints*/
    int          m_LargestLapSegmentIndex;
};

float Distance(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) +
                pow(y2 - y1, 2) * 1.0);
}

int main()
{
    int prevCheckpointX = 0;
    int prevCheckpointY = 0;
    int boostCycles = 0;
    GameWorld world;
    // game loop
    while (1) {
        Vec2 vehicle(0,0);
        Vec2 nextCpoint(0,0);
        Vec2 opponentVehicle(0,0);

        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint

        cin >> vehicle.x >> vehicle.y >> nextCpoint.x >> nextCpoint.y >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();

        cin >> opponentVehicle.x >> opponentVehicle.y; cin.ignore();

        if(world.GetSegmentsNum() == 0)
        {
            world.OnStartRace( Vec2((vehicle.x + opponentVehicle.x) * 0.5f, (vehicle.y + opponentVehicle.y) * 0.5f));
            prevCheckpointX = nextCpoint.x;
            prevCheckpointY = nextCpoint.y;
        }
        else if (nextCpoint.x != prevCheckpointX || nextCpoint.y != prevCheckpointY)
        {
            Vec2 cpoint = Vec2(prevCheckpointX, prevCheckpointY);
            world.OnCheckpointReached( cpoint );
            if(world.IsFirstLapCpointFound() && world.GetLargestLapSegmentIndex() == -1 )
            {
                world.OnFirstLapCompleted( cpoint );
            }

            prevCheckpointX = nextCpoint.x;
            prevCheckpointY = nextCpoint.y;
        }
        int current_segment = world.GetSegmentsNum() - 1;

        int boost_segment = -1;
        if(world.IsFirstLapCpointFound() && world.GetLargestLapSegmentIndex() >= 0)
        {
            boost_segment = world.GetLapEndingCpointIndex() + world.GetLargestLapSegmentIndex();
            if(boost_segment == current_segment && abs(nextCheckpointAngle) < 22)
            {
                boostCycles++;
            }
            else if(boostCycles > 0)
            {
                boostCycles -= 999999;
            }
        }
        float remainingDistanceToCpoint = Distance(nextCpoint.x, nextCpoint.y, vehicle.x, vehicle.y);
        float currentSegmentSize = world.GetLastCpoint().Distance(Vec2(nextCpoint.x, nextCpoint.y));
        bool should_boost = boostCycles > 0 && (remainingDistanceToCpoint > (currentSegmentSize * 0.5f));
        
        if(boost_segment >= 0)
        {
            cerr << "Current_Segment= " << current_segment << " boost expected in segment " << boost_segment <<  endl;
        }
        else 
        {
            cerr << "Current_Segment= " << current_segment  <<  endl;
        }
        if(should_boost)
        {
            cerr << "==============BOOST=============" << endl;
        }


        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        float nextCpointDist = static_cast<float>(nextCheckpointDist);
        float speedMax = 100.0f + ((nextCpointDist - 5000.f) / 1000.f) * M_PI;

        int thrust = clamp<int>( (speedMax - abs( static_cast<float>(nextCheckpointAngle * 0.71f))) * static_cast<float>((abs(nextCheckpointDist) + 600.f) / 2600.f), 5, 100);
        Vec2 center(8000, 4500);
        Vec2 d = center - nextCpoint;

        float length = d.Length();
        float dx = d.x / length;
        float dy = d.y / length;
        Vec2 prevCP = nextCpoint;
        nextCpoint.x += static_cast<int>(dx * thrust * M_PI);
        nextCpoint.y += static_cast<int>(dy * thrust * M_PI);
        cerr << "prevCP= " << prevCP.x << " " << prevCP.y << " nextCP= " << nextCpoint.x << " " << nextCpoint.y << " D=" << dx << " " << dy << endl;

        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        cout << nextCpoint.x << " " << nextCpoint.y ;
        if(should_boost)
        {
            cout << " BOOST" << endl;
        }
        else
        {
            cout << " " << thrust << endl;
        }
    }
}