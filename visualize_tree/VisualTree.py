import matplotlib.pylab as plt
import networkx as nx
import pygraphviz

# This example needs Graphviz and either PyGraphviz or pydot
# from networkx.drawing.nx_pydot import graphviz_layout
import regex as regex
from networkx.drawing.nx_agraph import graphviz_layout
from networkx.utils import pairwise


class VisualTree:
    tree_line = regex.compile(r"w=(\d+)\s.+\|((\s+\w\d\w\d)+)")
    attr_regex = regex.compile(r"\w=[\w\d\+\-]+")
    move_regex = regex.compile(r"\s+\w\d\w\d")
    def __init__(self, fname):
        self.G = nx.Graph()
        self.G.add_node(0, n=0)

        with open(fname) as f:
            lines = f.readlines()

        for line in lines:
            match = self.tree_line.match(line)
            if match:
                attrstr = match.group(0)
                moves = self.move_regex.findall(match.group(0))
                attrs = [atrr.split('=') for atrr in self.attr_regex.findall(attrstr)]
                attrs = {k:v for k,v in attrs}

                nx.add_path(self.G, [0] + moves )


                for i, a in enumerate([0] + moves):
                    self.G.add_node(
                        a,
                        label = a,
                        n=i,
                        **attrs
                    )

                self.G.add_node(
                    moves[-1], label=moves[-1] + f" {attrs['b']}" , n= len(moves), **attrs
                )

                if 'x' in attrs:
                    self.G.add_edge(
                        moves[-2], moves[-1], label=f" {attrs['x']}" , **attrs
                    )


    def __call__(self):

        pos = graphviz_layout(self.G, prog="twopi", args="", root=0)
        plt.figure(figsize=(18, 18))

        labels = nx.get_node_attributes(self.G, 'label')
        """ncolors = [
            "r" if a=="1" else "g"
            for a in nx.get_node_attributes(self.G, 'F')]
"""

        node_size = [20 if 'n' not in a else 300 - (int(a['n'])*10) for n, a in self.G.nodes(data=True)]
        nx.draw(self.G, pos, node_size=node_size, alpha=0.5, labels=labels, with_labels=True, font_weight='bold', font_size=13)


        edge_labels = nx.get_edge_attributes(self.G,'label') # key is edge, pls check for your case
        formatted_edge_labels = {(elem[0],elem[1]):edge_labels[elem] for elem in edge_labels} # use this to modify the tuple keyed dict if it has > 2 elements, else ignore
        nx.draw_networkx_edge_labels(self.G,pos,edge_labels=formatted_edge_labels,font_color='red', font_weight='bold')

        plt.axis("equal")
        plt.show()


if __name__=="__main__":
    VisualTree("../cmake-build-debug/moves.tree")()