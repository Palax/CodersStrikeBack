#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

#define MAPSIZEX 16000
#define MAPSIZEY 9000
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
float RadianToAngle(const float _rad)
{
    return _rad * 57.2957f;
}

struct Vec2 {
    Vec2() 
    : x(0), y(0) {}
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
    Vec2 AsVec2() { return Vec2(static_cast<int>(x), static_cast<int>(y)); } 
    /* Sp1 = segment point 1; Sp2 = segment point 2, p is the point, returns closest point*/
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
            cx = (_sp1.x + _sp2.x) * 0.5f;
            cy = (_sp1.y + _sp2.y) * 0.5f;
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

bool Collision(const Vec2f& _p, const Vec2f& _pv, const Vec2f& _o, const Vec2f& _ov, const int _podId1, const int _podId2)
{
    Vec2f fp = _p + (_pv * 2.f);
    Vec2f fo = _o + (_ov * 2.f);   
    float pSpd = _pv.Size() * 3.f;
    float relVel = RadianToAngle( acos( Vec2f::NormalizeDot(_pv, _ov) ) );

    Vec2f cpd = Vec2f::ComputeClosestPointOnLine(_p, fp, _o);

    Vec2f cpd2 = Vec2f::ComputeClosestPointOnLine(_p, fp, fo);
    Vec2f pcontact = cpd - _o;
    Vec2f pcontact2 = cpd2 - fo;
    float len1 = (cpd - _p).Size();
    float len2 = (cpd2 - _p).Size();
    float pod_size = 800.f;

    //     cerr << "_____________Pod_0" << _podId1 << " and Pod_0" << _podId2 << "_____________" << endl;
    //     cerr << "   ClosestPointOnALine(" << _p.x << " " << _p.y << ",  " << fp.x << " " << fp.y << ",  " << _o.x << " " << _o.y << ")=  " << (int)cpd.x << " " << (int)cpd.y << endl;    
    //     cerr << "   ClosestPointOnALine(" << _p.x << " " << _p.y << ",  " << fp.x << " " << fp.y << ",  " << fo.x << " " << fo.y << ")=  " << (int)cpd2.x << " " << (int)cpd2.y << endl;
    //     cerr << "   spdP = " << _pv.Size() << " relVel = " << relVel << "  <=> " << len1 << " < " << pSpd << " && " << pcontact.Size() << " < " << pod_size << " Result = " << (len1 < pSpd && pcontact.Size() < pod_size) << endl;
    //     cerr << "   spdO = " << _ov.Size() << " relVel = " << relVel << "  <=> " << len2 << " < " << pSpd << " && " << pcontact2.Size() << " < " << pod_size << " Result = " << (len2 < pSpd && pcontact2.Size() < pod_size) << endl;
    if( relVel < 90.f)
    {
        return false;
    }

    if( (len1 < pSpd && pcontact.Size() < pod_size) || (len2 < pSpd && pcontact2.Size() < pod_size) )
    {
        cerr << "   COLLISION IS IMMINENT BETWEEN POD_" << _podId1 << " AND POD_" << _podId2  << endl;
        return true;
    }
    return false;
}

struct Pod
{
    Pod(const int _nr)
    : x(0), y(0), vx(0), vy(0), angle(0), nextCheckpointId(-1), thrust(0), boost(false), shield(false), tx(0), ty(0), nr(_nr) {}

    int   x;
    int   y;
    int   vx;
    int   vy;
    int   angle;
    int   nextCheckpointId;
    int   thrust;
    bool  boost;
    bool  shield;
    int   tx;
    int   ty;
    const int nr;

    Vec2f GetPos() { return Vec2f(x, y); }
    Vec2f GetVel() { return Vec2f(vx, vy); }
    float GetSpeed() { return sqrt(vx * vx + vy * vy); }
    int GetRadius() const { return 400; }

    bool ReadInput() {
        cin >> x >> y >> vx >> vy >> angle >> nextCheckpointId; cin.ignore();
        cerr << "Reading Pod_0" << nr << " input with pos(" << x << ", " << y << ") Vel(" << vx << ", " << vy << ") " << " angle=" << angle << " nextCheckpoint=" << nextCheckpointId << endl;
        bool validRead = x > -MAPSIZEX && x < MAPSIZEX * 2 && y > -MAPSIZEY && y < MAPSIZEY * 2 && angle >= -360 && angle <= 360 && nextCheckpointId >= 0 && nextCheckpointId <= 12;
        if(!validRead)
        {
            cerr << "Game is updated with invalid pod parameters pos=(" << x << ", " << y << ") vel=(" << vx << ", " << vy << ") angle= " << angle << " nextChecpointId" << nextCheckpointId << endl;
            cerr << "Game will abort." << endl;
        }
        return validRead;
    }
    void WriteOutput() {
        cout << tx << " " << ty;
        if(shield)
        {
            cout << " SHIELD" << endl;
        }
        else if(boost)
        {
            cout << " BOOST" << endl;
        }
        else
        {
            cout << " " << thrust << endl;
        }
    }
};

class GameWorld
{
public:
    GameWorld()
    : m_CheckpointsNum(-1), m_LapsNum(-1), m_PPodA(1), m_PPodB(2), m_OPodA(3), m_OPodB(4), m_FrameCounter(0) {}

    /*Should be called only one time at the beginning of the race in order to retrieve game data about the map and the race*/
    bool ReadGameInput_RaceInitialization()
    {
        cin >> m_LapsNum; cin.ignore();
        cin >> m_CheckpointsNum; cin.ignore();
        if(m_LapsNum <= 0 || m_CheckpointsNum <= 0)
        {
            cerr << "Game is initialized with invalid race parameters, game will be aborted" << endl;
            return false;
        }
        cerr << "Map has " << m_LapsNum << " laps and " << m_CheckpointsNum << " checkpoints" << endl;

        m_Checkpoints.reserve(m_CheckpointsNum);
        for(int c = 0; c < m_CheckpointsNum; ++c)
        {
            Vec2 newCpoint(0,0);
            cin >> newCpoint.x >> newCpoint.y; cin.ignore();
            bool cpointIsWithinMapBounds = newCpoint.x >= 0 && newCpoint.x <= MAPSIZEX && newCpoint.y >= 0 && newCpoint.y <= MAPSIZEY; 
            if( cpointIsWithinMapBounds )
            {
                m_Checkpoints.push_back(newCpoint);
            }
            else
            {
                cerr << "Checkpoint(" << newCpoint.x << ", " << newCpoint.y << ") is not within map bounds, game will be aborted" << endl;
            }
        }
        m_PPodA.boost = true;
        return true;
    }
    bool ReadGameInput_RaceLoop()
    {
        return m_PPodA.ReadInput() && m_PPodB.ReadInput() && m_OPodA.ReadInput() && m_OPodB.ReadInput();
    }

    void ComputePodTarget(Pod& _pod)
    {
        Vec2f prevCheckpoint = _pod.nextCheckpointId <= 0 ? *m_Checkpoints.end() : m_Checkpoints[_pod.nextCheckpointId - 1];
        Vec2f nextCheckpoint = m_Checkpoints[_pod.nextCheckpointId];
        Vec2f nextOfNext = _pod.nextCheckpointId < (m_Checkpoints.size() - 1) ? m_Checkpoints[_pod.nextCheckpointId + 1] : nextCheckpoint;
        float nextCpointDist = (_pod.GetPos() - nextCheckpoint).Size();
        float speedMax = 100.0f + ((nextCpointDist - 5000.f) / 1000.f) * M_PI;
        float dpj = 1;
        if(_pod.GetSpeed() > 1)
        {
            dpj = Vec2f::NormalizeDot(_pod.GetVel(), nextCheckpoint - _pod.GetPos());
        }

        float pPodANextCPAngle = RadianToAngle( acos(dpj) ); 

        _pod.thrust = clamp<int>( (speedMax - abs( static_cast<float>(pPodANextCPAngle * 0.71f))) * static_cast<float>((abs(nextCpointDist) + 600.f) / 2600.f), 50, 100);
        _pod.tx = static_cast<int>(nextCheckpoint.x);
        _pod.ty = static_cast<int>(nextCheckpoint.y);

        Vec2f targetDiff = nextCheckpoint - _pod.GetPos();
        Vec2f td1 = targetDiff;
        Vec2f td2 = targetDiff;
        td1.MakePerpendicular(1);
        td2.MakePerpendicular(-1);
        Vec2f d1 = nextCheckpoint + td1;
        Vec2f d2 = nextCheckpoint + td2;
        Vec2f podADisp = _pod.GetPos() + _pod.GetVel();
        float distD1 = (d1 - podADisp).Size();
        float distD2 = (d2 - podADisp).Size();
        Vec2f dir1 = d1 - nextCheckpoint;
        Vec2f dir2 = d2 - nextCheckpoint;
        dir1.Normalize();
        dir2.Normalize();

        d1 = nextCheckpoint + (dir1 * _pod.GetSpeed());
        d2 = nextCheckpoint + (dir2 * _pod.GetSpeed());              
        if(distD1 > distD2)
        {
            _pod.tx = d1.x;
            _pod.ty = d1.y;
        }
        else
        {
            _pod.tx = d2.x;
            _pod.ty = d2.y;
        }
    }

    void UpdatePodABoost()
    {
        float distToNextCP = (Vec2f(m_Checkpoints[m_PPodA.nextCheckpointId]) - m_PPodA.GetPos()).Size();
        float segmentLen = 0;
        if(m_PPodA.nextCheckpointId == 0 || m_PPodA.nextCheckpointId >= m_CheckpointsNum)
        {
            segmentLen = (*m_Checkpoints.begin() - *m_Checkpoints.end()).Size(); 
        }
        else
        {
            segmentLen = (m_Checkpoints[m_PPodA.nextCheckpointId] - m_Checkpoints[m_PPodA.nextCheckpointId - 1]).Size();
        }
        
        if(distToNextCP < (segmentLen * 0.5f))
        {
            m_PPodA.boost = false;
        }
    }

    void UpdateRace()
    {
        m_PPodA.shield = false;
        m_PPodB.shield = false;

        ComputePodTarget(m_PPodA);
        ComputePodTarget(m_PPodB);
        UpdatePodABoost();

        bool selfCollide     = Collision(m_PPodA.GetPos(), m_PPodA.GetVel(), m_PPodB.GetPos(), m_PPodB.GetVel(), m_PPodA.nr, m_PPodB.nr);
        bool pod1WillCollide = Collision(m_PPodA.GetPos(), m_PPodA.GetVel(), m_OPodA.GetPos(), m_OPodA.GetVel(), m_PPodA.nr, m_OPodA.nr);
        pod1WillCollide     |= Collision(m_PPodA.GetPos(), m_PPodA.GetVel(), m_OPodB.GetPos(), m_OPodB.GetVel(), m_PPodA.nr, m_OPodB.nr);
        bool pod2WillCollide = Collision(m_PPodB.GetPos(), m_PPodB.GetVel(), m_OPodA.GetPos(), m_OPodA.GetVel(), m_PPodB.nr, m_OPodB.nr);
        pod2WillCollide     |= Collision(m_PPodB.GetPos(), m_PPodB.GetVel(), m_OPodB.GetPos(), m_OPodB.GetVel(), m_PPodB.nr, m_OPodB.nr);
        if(pod1WillCollide)
        {
            m_PPodA.shield = true;
        }
        if(pod2WillCollide)
        {
            m_PPodB.shield = true;
        }

        m_FrameCounter++;
    }
    void WriteGameOutput_RaceLoop()
    {
        m_PPodA.WriteOutput();
        m_PPodB.WriteOutput();
    }
private:
    /*These represents the positions of all checkpoints in a lap, in  a race with 3 laps and 4 checkpoints
      this vector will be of size 4*/
    vector<Vec2> m_Checkpoints;
    /*How many laps are for race completed*/
    int          m_LapsNum;//average of 3 or 4
    /*How many checkpoints are in a lap*/
    int          m_CheckpointsNum;//between 3 and 6 with average of 4
    uint         m_FrameCounter;//every frame will be added here, never a negative number
    Pod          m_PPodA;//Player's pod A
    Pod          m_PPodB;//Player's pod B
    Pod          m_OPodA;//Opponent's pod A
    Pod          m_OPodB;//Opponent's pod B
};

int main()
{
    GameWorld world;
    // Game init
    if( !world.ReadGameInput_RaceInitialization() )
    {
        return 0;
    }

    // Game loop
    while (1) 
    {
        if( !world.ReadGameInput_RaceLoop() ) {
            return 0;
        }

        world.UpdateRace();

        world.WriteGameOutput_RaceLoop();
    }
}