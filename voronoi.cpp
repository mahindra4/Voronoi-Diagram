#include <bits/stdc++.h>
using namespace std;
class Point{
public:
    double x,y;
    Point(double x = 0,double y = 0){
        this->x = x;
        this->y = y;
    }
    Point(const Point &p){
        x = p.x;
        y = p.y;
    }
    // Point(){
    //     // . .- - / ..... / ... - .- .-. / -.. --- / -. --- - .... .. -. --. / ---... -.--.-
    // }
    Point operator + (const Point &q){
        return Point(x+q.x,y+q.y);
    }
    Point operator - (const Point &q){
        return Point(x-q.x,y-q.y);
    }
    Point operator * (double k){
        return Point(x*k,y*k);
    }
    void print(){
        cout<<"x: "<<x<<" "<<"y: "<<y<<endl;
    }
    double det(Point q){
        return x*q.y-q.x*y;
    }
    bool equal(Point q){
        return x==q.x && y==q.y;
    }
};
class lineSegment{
public:    
    Point p,q;
    lineSegment(Point p,Point q){
        this->p = p;
        this->q = q;
    }
    lineSegment(){
        // eat 5 star do nothing :)
    }
    lineSegment(const lineSegment &l){
        p = l.p;
        q = l.q;
    }
    void print(){
        cout<<"Point1: "<<p.x<<" "<<p.y<<endl;
        cout<<"Point2: "<<q.x<<" "<<q.y<<endl;
    }
};
class Line{
public:
    // point - direction
    Point p,v; // p is the point, v is the direction of the vector 
    Line(Point p,Point v){
        this->p = p;
        this->v = v;
    }
    Line(){
        // eat 5 star do nothing :)
    }
    Line(const Line &l){
        p = l.p;
        v = l.v;

    }
    void print(){
        cout<<"Point: "<<p.x<<" "<<p.y<<endl;
        cout<<"direction: "<<v.x<<" "<<v.y<<endl;
    }
};

vector<Point> bb; // boundary box

// **************************************************************************************************************************************
//                                               LIBRARY FUNCION IMPLEMENTATIONS

double sqrt(double x){
    int t = 150;
    double l = 0,r = x;
    double ans = -1;
    while(t--){
        double mid = l+((r-l)/2);
        if(mid*mid >= x){
            ans = mid;
            r = mid - 1;
        }
        else{
            l = mid + 1;
        }
    }
    return ans;
}

// **************************************************************************************************************************************
int orentation(Point a,Point b,Point c){
    // gives the orentation of point c wrt the vector ab
    double val = (c.x-b.x)*(b.y-a.y)-(b.x-a.x)*(c.y-b.y);
    if(val == 0) return 0;
    return (val>0)?1:-1; // clockwise = +1 , anti-clockwise = -1, collinear = 0
}
bool is_contained(Point a,lineSegment l){
    // checks if point a lies in line   l
    Point p = l.p,q = l.q;
    return (a.x-p.x)*(q.y-p.y) == (q.x-p.x)*(a.y-p.y);
}
bool is_intersection(lineSegment l,lineSegment m){
    // checks whether there is an intersection between line l and m 
    Point a,b,c,d; // a -- b , c -- d (lines)
    a = l.p, b = l.q;
    c = m.p, d = m.q;
    int or_ab_c = orentation(a,b,c);
    int or_ab_d = orentation(a,b,d);
    int or_cd_a = orentation(c,d,a);
    int or_cd_b = orentation(c,d,b);
    if((or_ab_c != or_ab_d) && (or_cd_a != or_cd_b))  return true;
    if((or_ab_c == or_ab_d) && (or_ab_c == 0)){
        return is_contained(a,m) || is_contained(b,m);
    }
    return false;
}
bool is_intersection(Line l,lineSegment m){
    // its used for finding the intersection between a side of the cell (voronoi) and perpendicular bisector
    Point a,b,c,d;
    a = l.p;
    b = a+l.v;
    c = m.p;
    d = m.q;
    int or_ab_c = orentation(a,b,c);
    int or_ab_d = orentation(a,b,d);
    // return (or_ab_c != or_ab_d) || (or_ab_c == 0); // point of line segment is on either side (or) collinear with the line
    return (or_ab_c != or_ab_d); // if line segment m is lying on the line l then its not dividing the cell 
}
double distance(Point a,Point b){
    return sqrt(a.x*a.x+b.x*b.x);
}
Line Perpendicular_bisector(Point a,Point b){
    Point mid,dir;
    mid.x = (a.x+b.x)/2;
    mid.y = (a.y+b.y)/2;
    double d = distance(a,b);
    dir.x = -(b.y-a.y)/d;
    dir.y = (b.x-a.x)/d;
    Line l(mid,dir);
    return l;
}
Point intersection(Line l,lineSegment m){
    Point a,b,c,d;
    a = l.p;
    b = l.v;
    c = m.p;
    d = m.q-m.p;
    // a + r'*b = c + r*d 
    a = a-c; 
    double r = a.det(b)/d.det(b); // a is a vector (a -> c) so r is calculated
    return c+(d*r);
}
Point intersection(lineSegment l,lineSegment m){
    Point a,b,c,d;
    a = l.p;
    b = l.q-l.p;
    c = m.p;
    d = m.q-m.p;
    a = a-c;
    double r = a.det(b)/d.det(b); 
    return c+(d*r);
}
vector<vector<Point>> cell_division(vector<Point> cell,Line pb){
    // divides the cell into two regions wrt perpendicular bisector pb
    int n = cell.size();
    vector<Point> new_cell;
    vector<int> intersection_indices;
    Point prev_intersection;
    bool flag = 0; // indicates whether there is any previous intersection happened
    new_cell.push_back(cell[0]);
    for(int i=0;i<n;i++){
        int l = i;
        int r = (i+1)%n;
        lineSegment seg(cell[l],cell[r]);
        if(is_intersection(pb,seg)){
            Point intersection_pt = intersection(pb,seg);
            if(!intersection_pt.equal(cell[l])){
                new_cell.push_back(intersection_pt);
            }
            if(intersection_indices.empty() || intersection_indices.back()!=new_cell.size()-1)
                intersection_indices.push_back(new_cell.size()-1);
            if(i+1<n && !intersection_pt.equal(cell[r])){
                new_cell.push_back(cell[r]);
            }
        }
        else if(i+1<n){
            new_cell.push_back(cell[r]);
        }
    }
    if(intersection_indices.size()<2){
        return {cell,cell};
    }
    vector<Point> cell_1,cell_2;
    int l = intersection_indices[0],r = intersection_indices[1];
    n = new_cell.size();
    for(int i=0;i<n;i++){
        if(l<=i && i<=r)
            cell_1.push_back(new_cell[i]);
        if(i<=l || i>=r){
            cell_2.push_back(new_cell[i]);
        } 
    }
    return {cell_1,cell_2};
}
bool point_in_polygon(Point p,vector<Point> polygon){
    // checks whether point p is inside the polygon 
    // since voronoi cell is a convex polygon - vertex score = +1 , (not count 2 times since its on two edges)
    int intersection_cnt = 0;
    int n = polygon.size();
    Line ray(p,Point(1,0)); // a ray in x direction
    Point prev_intersection;
    bool flag = 0; // indicates whether there is any prev_intersection happened
    for(int i=0;i<n;i++){
        int l = i,r = (i+1)%n;
        lineSegment seg(polygon[l],polygon[r]);
        if(is_intersection(ray,seg)){
            Point intersection_pt = intersection(ray,seg);
            if(!flag || !prev_intersection.equal(intersection_pt)){
                // if there is no prev_intersection (or) current_intersecton != prev_intersection 
                prev_intersection = intersection_pt;
                if(intersection_pt.x >= p.x) intersection_cnt++;  // check if the ray really intersecting the line segment
                flag = 1;
            }
        }
    }
    return intersection_cnt&1;
}
vector<Point> voronoi_cell(int p,vector<Point> points){
    vector<Point> cell = bb;
    int n = points.size();
    for(int q=0;q<n;q++){
        if(p==q) continue;
        Line Perpendicular_bisector_pq = Perpendicular_bisector(points[p],points[q]);
        vector<vector<Point>> divided_cells = cell_division(cell,Perpendicular_bisector_pq);
        cell = divided_cells[0];
        if(point_in_polygon(points[p],divided_cells[1])){
            cell = divided_cells[1];
        }
    }
    return cell;
}
vector<vector<Point>> voronoi_tessellation(vector<Point> points){
    vector<vector<Point>> voronoi_cells;
    int n = points.size();
    for(int p=0;p<n;p++){
        voronoi_cells.push_back(voronoi_cell(p,points));
    }
    return voronoi_cells;
}
int main(){
    int n;
    cout<<"Enter number of points: ";
    cin>>n;
    vector<Point> points(n);
    for(int i=0;i<n;i++){
        int x,y;
        cin>>x>>y;
        points[i] = Point(x,y);
    }
    double x_min,x_max,y_min,y_max;
    x_min = x_max = points[0].x;
    y_min = y_max = points[0].y;
    for(int i=1;i<n;i++){
        x_min = min(x_min,points[i].x);
        x_max = max(x_max,points[i].x);
        y_min = min(y_min,points[i].y);
        y_max = max(y_max,points[i].y);
    }
    x_min--;
    x_max++;
    y_min--;
    y_max++;
    bb.push_back({x_min,y_min});
    bb.push_back({x_max,y_min});
    bb.push_back({x_max,y_max});
    bb.push_back({x_min,y_max});
    vector<vector<Point>> voronoi_partition = voronoi_tessellation(points);
    

    // for printing the cells
    // int i = 1;
    // cout<<"-----------------------------------";
    // for(auto &cell:voronoi_partition){
    //     cout<<"cell - "<<i<<" ----------------------\n";
    //     i++;
    //     for(auto &point:cell){
    //         point.print();
    //     }
    //     cout<<"-----------------------------------";
    // }

    ofstream out("voronoi_cells.json");
    out<<"{\n";
    out<<"  \"cells\": [\n";
    int sz = voronoi_partition.size();
    for(int i=0;i<sz;i++){
        out<<"    {\n";
        out<<"      \"cell\": "<<i+1<<",\n";
        out<<"      \"point\": {\"x\": "<<points[i].x<<", \"y\": "<<points[i].y<<"},\n";
        out<<"      \"region\": [\n";
        int m = voronoi_partition[i].size();
        for(int j=0;j<m;j++){
            out<<"        {\"x\": "<<voronoi_partition[i][j].x<<", \"y\": "<<voronoi_partition[i][j].y<<"}";
            if(j+1<m) out<<",";
            out<<"\n";
        }
        out<<"      ]\n";
        out<<"    }";
        if(i+1<sz) out<<",";
        out<<"\n";
    }
    out<<"  ],\n";
    out<<"  \"boundary_box\": {\n";
    out<<"    \"x_min\": "<<x_min<<",\n";
    out<<"    \"x_max\": "<<x_max<<",\n";
    out<<"    \"y_min\": "<<y_min<<",\n";
    out<<"    \"y_max\": "<<y_max<<"\n";
    out<<"  }\n";
    out<<"}\n";
    return 0;
}
