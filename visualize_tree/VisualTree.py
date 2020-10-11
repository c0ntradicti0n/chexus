import matplotlib.pylab as plt
import networkx as nx
import pygraphviz

# This example needs Graphviz and either PyGraphviz or pydot
# from networkx.drawing.nx_pydot import graphviz_layout
import regex as regex
from networkx.drawing.nx_agraph import graphviz_layout
from networkx.utils import pairwise





class VisualTree:
    tree_line = regex.compile(r"w=(\d+)\s.+\|((\s+[A-Za-z\_]+\w\d\w\d)+)")
    attr_regex = regex.compile(r"\w=[\w\d\+\-\_]+")
    move_regex = regex.compile(r"\s+[A-Za-z\_]+\w\d\w\d")
    def __init__(self, fname):
        self.G = nx.Graph()
        self.G.add_node(0, n=0)
        self.max_stufe = 0

        with open(fname) as f:
            lines = f.readlines()

        for line in lines:
            match = self.tree_line.match(line)
            if match:
                attrstr = match.group(0)
                moves = [0] + [str(i) + " " + m for i, m in enumerate( self.move_regex.findall(match.group(0)))]
                attrs = [atrr.split('=') for atrr in self.attr_regex.findall(attrstr)]
                _attrs = {k:v for k,v in attrs}

                f=int (_attrs['F'])

                for i, (a,b) in enumerate(pairwise(moves[::-1])):
                    attrs = _attrs
                    attrs['F'] = str(f)

                    self.G.add_edge(b,a, ** attrs,
                                    width=2,
                                    color='black'
                                    )



                    print (attrs)
                    self.G.add_node(
                        a,
                        label = str(a) + ( f"_{attrs['b']}" if (i==len(moves)-1) else " get"),
                        n=i,
                        color='black',
                        **attrs
                    )


                if (len(moves)) > self.max_stufe:
                    self.max_stufe = len(moves)

                if 'x' in attrs:
                    self.G.add_edge(
                        moves[-2], moves[-1], label=f" {attrs['x']}" , **attrs
                    )

                    if (attrs['x'] == 'PIVOT'):
                        self.G.edges[a,b]['color'] = 'red'
                        self.G.edges[a,b]['width'] = 10
                        self.G.nodes[b]['color'] = 'red'

                f *= -1


    def __call__(self):

        pos = graphviz_layout(self.G, prog="twopi", args="", root=0)
        plt.figure(figsize=(18, 18))

        labels = nx.get_node_attributes(self.G, 'label')
        """ncolors = [
            "r" if a=="1" else "g"
            for a in nx.get_node_attributes(self.G, 'F')]
"""
        edge_colors = [self.G[e[0]][e[1]]['color'] for e in self.G.edges() ]
        edge_widths = [self.G[e[0]][e[1]]['width'] for e in self.G.edges() ]

        node_colors = [self.G[n]['color'] if 'color' in self.G[n] else self.get_color_from_node(n) for n in self.G.nodes() ]


        node_size = [20 if 'n' not in a else 5000/(1+(int(a['n'])) **2) for n, a in self.G.nodes(data=True)]
        nx.draw(self.G, pos, node_size=node_size, alpha=0.5, labels=labels, edge_color = edge_colors,
                node_color=node_colors, width=edge_widths, with_labels=True, font_weight='bold', font_size=13)


        edge_labels = nx.get_edge_attributes(self.G,'label') # key is edge, pls check for your case

        formatted_edge_labels = {(elem[0],elem[1]):edge_labels[elem] for elem in edge_labels} # use this to modify the tuple keyed dict if it has > 2 elements, else ignore
        nx.draw_networkx_edge_labels(self.G,pos,edge_labels=formatted_edge_labels,  font_color='red', font_weight='bold')

        plt.axis("equal")
        plt.show()

    def get_color_from_node(self, n):
        if n==0:
            return "orange"
        if 'F' in self.G.nodes[n]:
            if int(self.G.nodes[n]['F']) > 0:
                return "green"
            else:
                return "yellow"




if __name__=="__main__":

    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("treefile")
    args = parser.parse_args()
    print(args.treefile)
    VisualTree(args.treefile)()