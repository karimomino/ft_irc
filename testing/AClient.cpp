class AClient
{
private:
    int x;
public:
    AClient(void);
    virtual ~AClient() = 0;
};

AClient::AClient(void)
{
}

AClient::~AClient()
{
}

class Inh : public AClient
{
private:
    /* data */
public:
    Inh(AClient & x);
    ~Inh();
};

Inh::Inh(AClient & x) : AClient(x)
{
}

Inh::~Inh()
{
}
