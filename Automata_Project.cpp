#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

class NFAState
{
public:
    vector<int> transitions[2], epsilonTransitions;
    bool f = 0;
};

class DFAState
{
public:
    int transitions[2] = {-1, -1};
    bool f = 0;
};

// Inserting '.' if not present
string insert_concat(string regexp)
{
    string ret = "";
    char c, c2;
    for (int i = 0; i < regexp.size(); i++)
    {
        c = regexp[i];
        if (i + 1 < regexp.size())
        {
            c2 = regexp[i + 1];
            ret += c;
            if (c != '(' && c2 != ')' && c != '|' && c2 != '|' && c2 != '*' && c2 != '+')
            {
                ret += '.';
            }
        }
    }
    ret += regexp[regexp.size() - 1];
    return ret;
}

// Precedence order
int priority(char c)
{
    switch (c)
    {
    case '*':
        return 4;
    case '+':
        return 3;
    case '.':
        return 2;
    case '|':
        return 1;
    default:
        return 0;
    }
}

string infix_to_postfix(string regexp)
{
    string postfix = "";
    stack<char> op;
    char c;
    for (int i = 0; i < regexp.size(); i++)
    {
        switch (regexp[i])
        {
        case 'a':
        case 'b':
            postfix += regexp[i];
            break;
        case '(':
            op.push(regexp[i]);
            break;
        case ')':
            while (op.top() != '(')
            {
                postfix += op.top();
                op.pop();
            }
            op.pop();
            break;
        default:
            while (!op.empty())
            {
                c = op.top();
                if (priority(c) >= priority(regexp[i]))
                {
                    postfix += op.top();
                    op.pop();
                }
                else
                    break;
            }
            op.push(regexp[i]);
        }
        // cout<<regexp[i]<<' '<<postfix<<endl;
    }
    while (!op.empty())
    {
        postfix += op.top();
        op.pop();
    }
    return postfix;
}

void epsilon_closure(int state, set<int> &si, vector<NFAState> &nfa)
{
    for (int i = 0; i < nfa[state].epsilonTransitions.size(); i++)
    {
        if (si.count(nfa[state].epsilonTransitions[i]) == 0)
        {
            si.insert(nfa[state].epsilonTransitions[i]);
            epsilon_closure(nfa[state].epsilonTransitions[i], si, nfa);
        }
    }
}

set<int> state_change(int c, set<int> &si, vector<NFAState> &nfa)
{
    set<int> temp;
    if (c == 1)
    {
        for (std::set<int>::iterator it = si.begin(); it != si.end(); ++it)
        {
            for (int j = 0; j < nfa[*it].transitions[0].size(); j++)
            {
                temp.insert(nfa[*it].transitions[0][j]);
            }
        }
    }
    else
    {
        for (std::set<int>::iterator it = si.begin(); it != si.end(); ++it)
        {
            for (int j = 0; j < nfa[*it].transitions[1].size(); j++)
            {
                temp.insert(nfa[*it].transitions[1][j]);
            }
        }
    }
    return temp;
}

// NFA to DFA
vector<DFAState> constructDFA(string postfix)
{

    // ε NFA construction
    vector<NFAState> nfa;
    int nfa_size = 0;
    class NFAState init_nfa_state;

    vector<DFAState> dfa;
    int dfa_size = 0;
    class DFAState init_dfa_state;

    stack<int> st;
    for (int i = 0; i < postfix.size(); i++)
    {
        if (postfix[i] == 'a' || postfix[i] == 'b')
        {
            nfa.push_back(init_nfa_state);
            nfa.push_back(init_nfa_state);
            nfa[nfa_size].transitions[postfix[i] - 'a'].push_back(nfa_size + 1);
            st.push(nfa_size);
            st.push(nfa_size + 1);
            nfa_size += 2;
        }

        else if (postfix[i] == '*')
        {
            nfa.push_back(init_nfa_state);
            nfa.push_back(init_nfa_state);
            int b = st.top();
            st.pop();
            int a = st.top();
            st.pop();
            nfa[nfa_size].epsilonTransitions.push_back(a);
            nfa[nfa_size].epsilonTransitions.push_back(nfa_size + 1);
            nfa[b].epsilonTransitions.push_back(a);
            nfa[b].epsilonTransitions.push_back(nfa_size + 1);
            st.push(nfa_size);
            st.push(nfa_size + 1);
            nfa_size += 2;
        }

        else if (postfix[i] == '+')
        {
            nfa.push_back(init_nfa_state);
            nfa.push_back(init_nfa_state);
            int b = st.top();
            st.pop();
            int a = st.top();
            st.pop();
            nfa[nfa_size].epsilonTransitions.push_back(a); // At least one repetition
            nfa[b].epsilonTransitions.push_back(a);        // Loop back for more repetitions
            nfa[b].epsilonTransitions.push_back(nfa_size + 1);
            st.push(nfa_size);
            st.push(nfa_size + 1);
            nfa_size += 2;
        }

        else if (postfix[i] == '.')
        {
            int d = st.top();
            st.pop();
            int c = st.top();
            st.pop();
            int b = st.top();
            st.pop();
            int a = st.top();
            st.pop();
            nfa[b].epsilonTransitions.push_back(c);
            st.push(a);
            st.push(d);
        }

        else if (postfix[i] == '|')
        {
            nfa.push_back(init_nfa_state);
            nfa.push_back(init_nfa_state);
            int d = st.top();
            st.pop();
            int c = st.top();
            st.pop();
            int b = st.top();
            st.pop();
            int a = st.top();
            st.pop();
            nfa[nfa_size].epsilonTransitions.push_back(a);
            nfa[nfa_size].epsilonTransitions.push_back(c);
            nfa[b].epsilonTransitions.push_back(nfa_size + 1);
            nfa[d].epsilonTransitions.push_back(nfa_size + 1);
            st.push(nfa_size);
            st.push(nfa_size + 1);
            nfa_size += 2;
        }
    }

    int final_state = st.top();
    st.pop();
    int start_state = st.top();
    st.pop();
    // cout<<start_state<<' '<<final_state<<endl;
    nfa[final_state].f = 1;

    // NFA to DFA
    set<int> si;
    queue<set<int>> que;
    map<set<int>, int> mp;
    mp[si] = -1;
    set<int> temp1;
    set<int> temp2;
    int ct = 0;
    si.clear();
    si.insert(0);
    epsilon_closure(start_state, si, nfa);
    if (mp.count(si) == 0)
    {
        mp[si] = ct++;
        que.push(si);
    }
    int p = 0;
    bool f1 = false;
    while (que.size() != 0)
    {
        dfa.push_back(init_dfa_state);
        si.clear();
        si = que.front();
        f1 = false;
        for (set<int>::iterator it = si.begin(); it != si.end(); ++it)
        {
            if (nfa[*it].f == true)
                f1 = true;
        }
        dfa[p].f = f1;
        temp1 = state_change(1, si, nfa);
        si = temp1;
        for (set<int>::iterator it = si.begin(); it != si.end(); ++it)
        {
            epsilon_closure(*it, si, nfa);
        }
        if (mp.count(si) == 0)
        {
            mp[si] = ct++;
            que.push(si);
            dfa[p].transitions[0] = ct - 1;
        }
        else
        {
            dfa[p].transitions[0] = mp.find(si)->second;
        }
        temp1.clear();

        si = que.front();
        temp2 = state_change(2, si, nfa);
        si = temp2;
        for (set<int>::iterator it = si.begin(); it != si.end(); ++it)
        {
            epsilon_closure(*it, si, nfa);
        }
        if (mp.count(si) == 0)
        {
            mp[si] = ct++;
            que.push(si);
            dfa[p].transitions[1] = ct - 1;
        }
        else
        {
            dfa[p].transitions[1] = mp.find(si)->second;
        }
        temp2.clear();
        que.pop();
        p++;
    }
    for (int i = 0; i < p; i++)
    {
        if (dfa[i].transitions[0] == -1)
            dfa[i].transitions[0] = p;
        if (dfa[i].transitions[1] == -1)
            dfa[i].transitions[1] = p;
    }
    dfa.push_back(init_dfa_state);
    dfa[p].transitions[0] = p;
    dfa[p].transitions[1] = p;
    // cout<<p<<endl;

    return dfa;
}

// Function to minimize DFA
pair<int, vector<tuple<int, int, bool>>> minimize_dfa(vector<DFAState> dfa)
{
    // cout<<dfa.size()<<endl;
    // Hopcroft's Algorithm
    vector<int> grp(dfa.size());
    vector<vector<int>> part(2, vector<int>());

    part[0].push_back(0);
    for (int i = 1; i < (int)grp.size(); i++)
    {
        if (dfa[i].f == dfa[0].f)
        {
            grp[i] = 0;
            part[0].push_back(i);
        }
        else
        {
            grp[i] = 1;
            part[1].push_back(i);
        }
    }

    if (!part[1].size())
        part.erase(part.end());

    bool chk = true;
    int strt = 0;
    while (chk)
    {
        chk = false;
        for (int i = 0; i < part.size(); i++)
        {
            for (int j = 0; j < 2; j++)
            {
                vector<pair<int, int>> trans(part[i].size());
                for (int k = 0; k < part[i].size(); k++)
                {
                    if (dfa[part[i][k]].transitions[j] >= 0)
                        trans[k] = make_pair(grp[dfa[part[i][k]].transitions[j]], part[i][k]);
                    else
                        trans[k] = make_pair(-1, part[i][k]);
                }
                sort(trans.begin(), trans.end());
                if (trans[0].first != trans[trans.size() - 1].first)
                {
                    chk = true;

                    int k, m = part.size() - 1;

                    part[i].clear();
                    part[i].push_back(trans[0].second);
                    for (k = 1; k < trans.size() && (trans[k].first == trans[k - 1].first); k++)
                    {
                        part[i].push_back(trans[k].second);
                    }

                    while (k < trans.size())
                    {
                        if (trans[k].first != trans[k - 1].first)
                        {
                            part.push_back(vector<int>());
                            m++;
                        }
                        grp[trans[k].second] = m;
                        part[m].push_back(trans[k].second);
                        k++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < part.size(); i++)
    {
        for (int j = 0; j < part[i].size(); j++)
        {
            if (part[i][j] == 0)
                strt = i;
        }
    }

    vector<tuple<int, int, bool>> ret(part.size());
    vector<int> newOrder(part.size(), -1);

    int newIndex = 0;
    newOrder[strt] = newIndex++;

    for (int i = 0; i < (int)part.size(); i++)
    {
        if (i != strt)
            newOrder[i] = newIndex++;
    }

    for (int i = 0; i < (int)part.size(); i++)
    {
        get<0>(ret[newOrder[i]]) = (dfa[part[i][0]].transitions[0] >= 0) ? newOrder[grp[dfa[part[i][0]].transitions[0]]] : -1;
        get<1>(ret[newOrder[i]]) = (dfa[part[i][0]].transitions[1] >= 0) ? newOrder[grp[dfa[part[i][0]].transitions[1]]] : -1;
        get<2>(ret[newOrder[i]]) = dfa[part[i][0]].f;
    }

    return make_pair(0, ret);
}

vector<int> print(vector<tuple<int, int, bool>> min_dfa, vector<int> v_reg)
{
    int i;
    cout << "\nMinimized DFA Table:--" << endl;
    cout << "-------------------------------------------" << endl;
    cout << " States |\ta\t|\tb\t|  Final State\t|" << endl;
    cout << "-------------------------------------------" << endl;
    for (i = 0; i < (int)min_dfa.size(); i++)
    {
        cout << "\t" << i << "\t|\t";
        v_reg.push_back(get<0>(min_dfa[i]));
        cout << get<0>(min_dfa[i]) << "\t|\t";

        v_reg.push_back(get<1>(min_dfa[i]));
        cout << get<1>(min_dfa[i]) << "\t|\t";

        v_reg.push_back(get<2>(min_dfa[i]));
        if (get<2>(min_dfa[i]))
            cout << "\tYes\t\t|";
        else
            cout << "\tNo \t\t|";
        cout << endl;
    }
    cout << "--------------------------------------------" << endl;
    cout << "Initial State is: " << 0;
    return v_reg;
}

vector<int> dfa_main(string regexp)
{
    vector<int> v_reg;

    regexp = insert_concat(regexp);
    string postfix = infix_to_postfix(regexp);

    cout << "\nPostfix Expression for Regex: " << regexp << " is " << postfix;

    vector<DFAState> dfa = constructDFA(postfix);

    pair<int, vector<tuple<int, int, bool>>> min_dfa_tmp = minimize_dfa(dfa);

    vector<tuple<int, int, bool>> min_dfa = min_dfa_tmp.second;
    int start_st = min_dfa_tmp.first;

    vector<int> result = print(min_dfa, v_reg);
    cout << endl
         << endl;

    return result;
}

// Function to simulate DFA on a given string
bool simulate_dfa(const vector<tuple<int, int, bool>> &dfa, int start_state, const string &input)
{
    int current_state = start_state;
    for (char c : input)
    {
        int transition = (c == 'a') ? 0 : 1;
        if (transition == 0)
        {
            current_state = (std::get<0>(dfa[current_state]) >= 0) ? std::get<0>(dfa[current_state]) : -1;
        }
        else if (transition == 1)
        {
            current_state = (std::get<1>(dfa[current_state]) >= 0) ? std::get<1>(dfa[current_state]) : -1;
        }
        else
        {
            current_state = -1;
        }
        if (current_state == -1)
            return false;
    }
    return get<2>(dfa[current_state]);
}

// Function to find distinct strings that satisfy one regex but not the other
void generate_distinct_strings(
    const vector<tuple<int, int, bool>> &dfa1, int start1,
    const vector<tuple<int, int, bool>> &dfa2, int start2, int n)
{
    queue<string> q;
    set<string> visited;
    vector<string> results;

    q.push("");

    while (!q.empty() && results.size() < n)
    {
        string current = q.front();
        q.pop();

        if (visited.count(current))
            continue;
        visited.insert(current);

        bool in_dfa1 = simulate_dfa(dfa1, start1, current);
        bool in_dfa2 = simulate_dfa(dfa2, start2, current);

        if (in_dfa1 != in_dfa2)
        {
            results.push_back(current);
        }

        if (results.size() < n)
        {
            q.push(current + "a");
            q.push(current + "b");
        }
    }

    if (results.empty())
    {
        cout << "No distinct strings found to prove inequivalence." << endl;
    }
    else
    {
        cout << n << " Distinct strings for proving their inequivalence:" << endl;
        for (const string &s : results)
        {
            if (s == "")
                cout << "ε" << endl;
            else
                cout << s << endl;
        }
    }
}

int main()
{
    string regexp1, regexp2;
    cout << "Enter two Regular Expressions:-\n(only combination of 'a' 'b' '|' '.' '*' '+' '(' ')' are allowed)\neg- a.b, a+, aa*b|b, (ab|b*).a\n"
         << endl;
    cout << "Regular Expression 1: ";
    cin >> regexp1;
    cout << "Regular Expression 2: ";
    cin >> regexp2;

    vector<int> result1 = dfa_main(regexp1);
    vector<int> result2 = dfa_main(regexp2);

    if (result1 == result2)
        cout << "Both Regular Expressions are equivalent." << endl;
    else
    {
        cout << "Both Regular Expressions are not equivalent." << endl;
        int n;
        cout << "\nEnter no. of distinct strings to be generated- ";
        cin >> n;
        vector<DFAState> dfa1 = constructDFA(infix_to_postfix(insert_concat(regexp1)));
        vector<DFAState> dfa2 = constructDFA(infix_to_postfix(insert_concat(regexp2)));

        pair<int, vector<tuple<int, int, bool>>> min_dfa1 = minimize_dfa(dfa1);
        pair<int, vector<tuple<int, int, bool>>> min_dfa2 = minimize_dfa(dfa2);

        generate_distinct_strings(min_dfa1.second, min_dfa1.first, min_dfa2.second, min_dfa2.first, n);
    }
    return 0;
}