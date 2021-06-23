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
float RadianToAngle(const float _rad)
{
    return _rad * 57.2957f;
}

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
    float Size() const{
        return sqrt(x * x + y * y);
    } 
    int x;
    int y;
    static const Vec2 sZero;
};

struct Vec2f
{
    Vec2f(const float _x, const float _y) 
    : x(_x), y(_y)  {}
    Vec2f(const int _x, const int _y) 
    : x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}
    Vec2f(const Vec2& _v) 
    : x(static_cast<float>(_v.x)), y(static_cast<float>(_v.y)) {}
    Vec2f(const double _x, const double _y) 
    : x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}

    static Vec2f NormalizeFrom(const float _x, const float _y){
        float length = sqrt((_x * _x) + (_y * _y));
        return Vec2f(static_cast<float>(_x) / length, static_cast<float>(_y) / length);
    }
    float Size() const{
        return sqrt(x * x + y * y);
    }
    static float Dot(const Vec2f& _a, const Vec2f& _b)
    {
        return (_a.x * _b.x) + (_a.y * _b.y);
    }
    static float NormalizeDot(const Vec2f& _a, const Vec2f& _b) 
    {
        Vec2f a = Vec2f::NormalizeFrom(_a.x, _a.y);
        Vec2f b = Vec2f::NormalizeFrom(_b.x, _b.y);
        return Dot(a, b);
    }
    Vec2f operator-(const Vec2f& _other) const
    {
        return Vec2f(x - _other.x, y - _other.y);
    }
    Vec2f operator+(const Vec2f& _other) const
    {
        return Vec2f(x + _other.x, y + _other.y);
    }    
    Vec2f operator/(const float _val) const
    {
        return Vec2f(x / _val, y / _val);
    }
    Vec2f operator*(const float _val) const
    {
        return Vec2f(x * _val, y * _val);
    }    
    static Vec2f ComputeClosestPointOnLine(const Vec2f& _sp1, const Vec2f& _sp2, const Vec2f& _p)
    {
        float a1 = _sp2.y - _sp1.y;
        float b1 = _sp1.x - _sp2.x;
        double c1 = (_sp2.y - _sp1.y) * _sp1.x + (_sp1.x - _sp2.x) * _sp1.y;
        double c2 = (-b1 * _p.x) + (a1 * _p.y);
        double det = a1 * a1 - (-b1 * b1);
        double cx = 0;
        double cy = 0;
        if( det != 0)
        {
            cx = static_cast<float>((a1 * c1 - b1 * c2) / det);
            cy = static_cast<float>((a1 * c2 - (-b1 * c1)) / det);
        } else {
            cx = _p.x;
            cy = _p.y;
        }
        return Vec2f(cx, cy);
    }
    void MakePerpendicular(const int _sign)
    {
        float sign = clamp<float>(_sign, -1.f, 1.f);
        float tx = x;
        float ty = y;
        x = ty * sign;
        y = tx * (sign * -1.f);
    }
    void Normalize()
    {
        float length = Size();
        x /= length;
        y /= length;
    } 

    float x;
    float y;
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
    Vec2 GetNextCpoint(const Vec2& _vCurrentCpoint)
    {
        for(int i = 0; i < m_CpointsPositions.size() - 1; ++i)
        {
            if(m_CpointsPositions[i].IsNear(_vCurrentCpoint))
            {
                return m_CpointsPositions[i+1];
            }
        }
        return Vec2::sZero;
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

bool AreCirclesInCollision(const Vec2f& _c1, const Vec2f& _c2)
{
    float dist = abs((_c1.x - _c2.x) * (_c1.x - _c2.x) + (_c1.y - _c2.y) * (_c1.y - _c2.y));
    return dist < 640000.f;
}

bool Collision(const Vec2& _p, const Vec2& _o, const Vec2f& _pv, const Vec2f& _ov )
{
    Vec2f nopos = Vec2f(_o);
    Vec2f np = Vec2f(_p) + (_pv * 2.f);
    Vec2f no = Vec2f(_o) + (_ov * 2.f);
    if(AreCirclesInCollision(np, no) && _pv.Size() > 500.f)
    {
        cerr << "WILL COLIDE NEXT FRAME" << endl;
        return true;
    }
    return false;
}

int main()
{
    int prevCheckpointX = 0;
    int prevCheckpointY = 0;
    int boostCycles = 0;
    GameWorld world;
    Vec2 lastVehicle(0,0);
    Vec2 lastOpponentVehicle(0,0);
    Vec2 prevReachedCpoint(0,0);
    // game loop
    while (1) 
    {
        Vec2 vehicle(0,0);
        Vec2 nextCpoint(0,0);
        Vec2 opponentVehicle(0,0);

        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint

        cin >> vehicle.x >> vehicle.y >> nextCpoint.x >> nextCpoint.y >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();

        cin >> opponentVehicle.x >> opponentVehicle.y; cin.ignore();

        if(world.GetSegmentsNum() == 0)
        {
            Vec2 startCpoint = Vec2((vehicle.x + opponentVehicle.x) * 0.5f, (vehicle.y + opponentVehicle.y) * 0.5f);
            world.OnStartRace( startCpoint );
            prevReachedCpoint = startCpoint;
            prevCheckpointX = nextCpoint.x;
            prevCheckpointY = nextCpoint.y;
        }
        else if (nextCpoint.x != prevCheckpointX || nextCpoint.y != prevCheckpointY)
        {
            Vec2 cpoint = Vec2(prevCheckpointX, prevCheckpointY);
            prevReachedCpoint = cpoint;
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
            if(boost_segment == current_segment && abs(nextCheckpointAngle) < 18)
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
        bool should_boost = boostCycles > 0 && (remainingDistanceToCpoint > (currentSegmentSize * 0.35f));
        
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

        Vec2 vehicleVelocity(vehicle.x - lastVehicle.x, vehicle.y - lastVehicle.y);
        Vec2 opponentVelocity(opponentVehicle.x - lastOpponentVehicle.x, opponentVehicle.y - lastOpponentVehicle.y);

        Vec2 center = Vec2(8000, 4500);
        Vec2 nextOfNext = world.GetNextCpoint(nextCpoint);
        if(!nextOfNext.IsEqual(Vec2::sZero))
        {
            center = (center + nextOfNext) * 0.5f;
        }
        Vec2 cton = center - nextCpoint;

        float length = cton.Size();
        float dx = cton.x / length;
        float dy = cton.y / length;
        Vec2 prevCP = nextCpoint;

        Vec2f dirToNCp = nextCpoint - vehicle;
        Vec2f d1 = dirToNCp;
        Vec2f d2 = dirToNCp;
        d1.MakePerpendicular(1.f);
        d2.MakePerpendicular(-1.f);
        d1 = Vec2f(nextCpoint.x + d1.x, nextCpoint.y + d1.y); 
        d2 = Vec2f(nextCpoint.x + d2.x, nextCpoint.y + d2.y); 

        Vec2f vn = vehicle + vehicleVelocity;

        float dist1 = (d1 - vn).Size();
        float dist2 = (d2 - vn).Size();
        Vec2f dir1 = Vec2f(d1.x - nextCpoint.x, d1.y - nextCpoint.y);
        Vec2f dir2 = Vec2f(d2.x - nextCpoint.x, d2.y - nextCpoint.y);
        dir1.Normalize();
        dir2.Normalize();

        d1 = Vec2f(nextCpoint) + (dir1 * vehicleVelocity.Size());
        d2 = Vec2f(nextCpoint) + (dir2 * vehicleVelocity.Size());

        if(dist1 > dist2)
        {
            nextCpoint = Vec2(static_cast<int>(d1.x), static_cast<int>(d1.y));
        }
        else
        {
            nextCpoint = Vec2(static_cast<int>(d2.x), static_cast<int>(d2.y));
        }

        bool should_shield = Collision(vehicle, opponentVehicle, vehicleVelocity, opponentVelocity);

        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        cout << nextCpoint.x << " " << nextCpoint.y ;
        if(should_shield)
        {
            cout << " SHIELD" << endl;
        }
        else if(should_boost)
        {
            cout << " BOOST" << endl;
        }
        else
        {
            cout << " " << thrust << endl;
        }

        //These should always be the last to be set here
        lastVehicle = vehicle;
        lastOpponentVehicle = opponentVehicle;
    }
}