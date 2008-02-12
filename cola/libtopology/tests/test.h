
template <typename T>
T getRand(T range) {
    double d=static_cast<double>(rand())/static_cast<double>(RAND_MAX);
    T r=static_cast<T>(static_cast<double>(range)*d);
	return r;
}

void generateRandomRects(unsigned n, vpsc::Rectangles &rs) {
	rs.resize(n);
	const int rect_size = 50;
	const int fld_size = static_cast<int>(
            sqrt(static_cast<double>(rect_size * n) / 2.0));
	int coords[4];
	for (unsigned i = 0; i < n; ++i) {
		for (unsigned d = 0; d < 2; ++d) {
			//unsigned const end = 1 + (rand() % (fld_size - 1));
			//unsigned const start = rand() % end;
			int const start = getRand(fld_size);
			int const end = start + getRand(rect_size) + 1;
			coords[2 * d] = start;
			coords[2 * d + 1] = end;
		} 
		rs[i]=new vpsc::Rectangle(coords[0],coords[1],coords[2],coords[3]);
	}
}

Node* addNode(Nodes& vs, double x, double y, double w, double h) {
    vpsc::Rectangle* r = new vpsc::Rectangle(x,x+w,y,y+h);
    Node *v = new Node(vs.size(), r);
    vs.push_back(v);
    return v;
}
void addToPath(EdgePoints& ps, Node *v, topology::EdgePoint::RectIntersect i) {
    ps.push_back(new EdgePoint(v,i));
}

void writeFile(const Nodes& vs, const Edges& es, const string& outputFileName) {
    const unsigned n=vs.size();
    vector<cola::Edge> cedges;

    for(unsigned i=0;i<es.size();i++) {
        cedges.push_back(make_pair(1,2));
    }

    vector<straightener::Route*> routes;
    for(vector<Edge*>::const_iterator e=es.begin();e!=es.end();++e) {
        routes.push_back((*e)->getRoute());
    }

    vector<string> labels(n);
    for(unsigned i=0;i<n;++i) {
        stringstream ss;
        ss << i;
        labels[i]=ss.str();
    }

    vector<vpsc::Rectangle*> rs;
    for(Nodes::const_iterator i=vs.begin();i!=vs.end();++i) {
	    rs.push_back((*i)->rect);
    }
    OutputFile of(rs,cedges,NULL,outputFileName.c_str(),true,false);
    of.setLabels(labels);
    of.routes=&routes;
    of.generate();

    for_each(routes.begin(),routes.end(),delete_object());
}
void check(const TopologyConstraints& t, valarray<double>& g, cola::SparseMap& h,
        double* eg, double* eh) {
    for(unsigned i=0;i<t.n;++i) {
        //printf("g[%d]=%f,eg[%d]=%f\n",i,g[i],i,expectedG[i]);
        assert(fabs(g[i]-eg[i])<1e-4);
        for(unsigned j=0;j<t.n;++j) {
            //printf("h[%d,%d]=%f,eh[%d,%d]=%f\n",i,j,h(i,j),i,j,expectedH[i*n+j]);
            //printf("%f,",h(i,j));
            assert(fabs(h(i,j)-eh[i*t.n+j])<1e-4);
        }
        printf("\n");
    }
}