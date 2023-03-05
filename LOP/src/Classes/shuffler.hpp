#pragma once

class shuffler {
    private:
        unsigned short m_pos;
        unsigned short* m_perm;
    
    public:
        shuffler(const unsigned short& n);
        shuffler(const unsigned short& n, const unsigned short& i);
        ~shuffler();

        const unsigned short& next();
        bool empty();
};