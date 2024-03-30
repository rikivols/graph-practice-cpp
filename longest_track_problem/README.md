# Longest skying track

This problem is to practice working with graphs, topological sorting and DFS.

## Description
Many of you have likely noticed the successes of Ester Ledecka at the Winter Olympics in 2018, when she, to the amazement of almost the entire world, not just the sporting community, managed to win a gold medal in alpine skiing. BBC even described Ledecka's victory as "one of the most amazing Olympic stories of all time." As it happens in our country, as soon as our representatives start doing well in any discipline, the entire nation becomes not only complete experts but also (semi-)professional athletes. After Nagano, we had 10 million hockey players, then everyone became speed skaters (by the way, do you know if they've built that rink for Martina yet?). Later, there was a shift to biathlon, and now the reigning king is alpine skiing.

Devil take it, one of your acquaintances, let's call him Towman for the moment, thought to make some money from this skiing enthusiasm. It didn't take long for Towman to come up with a specific idea – to organize the Czech Championship in alpine skiing at his ski resort in Osvetamane. Why the raised eyebrows? Are you surprised that someone allowed a downhill race to be organized in a ski area with the longest slope being 400 meters and a vertical drop of 98 meters? Even though the minimum length of the track according to official rules is 800 m? That’s not a problem for Towman; he can handle even bigger things!

But seriously. In reality, it is a bit of a problem. It could easily happen that one of the racers would complain and the race would have to be cancelled due to irregularities. And that would mean not only wasted preparations, disappointed fans, but also the need to return the grant Towman received for the race. And nobody wants that, right?

When you learned about the problem, a solution immediately came to mind. The resort in Osvetamane consists of several slopes. Perhaps if it were possible to somehow connect them into one long slope, we might reach the limit. Towman liked your idea, of course, and immediately asked you to plan the longest possible track.

## Program Interface
Your task is to implement the function std::vector<Path> longest_track(size_t points, const std::vector<Path>& all_paths), which finds the longest possible track for a ski racing competition in the given map. The track can start and end at any junction. Parameters are the number of junctions points and the list of usable paths between junctions all_paths. Each path contains a starting junction from and a target to and can only be traversed in this direction, because all sections must lead downhill. It also contains the length of the given path length. The length is always a non-negative integer.

The output is a list of consecutive paths (i.e., the to of one path is the from of the following), forming the longest possible track. If there are multiple longest tracks, any of them can be returned.

It can be assumed that the input is always valid, that is:

* Junctions at which paths start and end are from the range [0, points).
* All paths lead downhill.

## Scoring Conditions

* To earn 3 points, the task must be correctly solved for very small instances. All paths are 10 or less in length.
* To earn 5 or 7 points, larger instances must be correctly solved, but the lengths of individual paths are still limited to 10.
* To earn 10 points, large instances containing very long paths must be solved correctly and quickly. However, the length of the permissible track still does not exceed the range of the int data type.

Time limits are 13 seconds for the basic test and 6, 11, and 5 seconds for individual speed tests. In addition, all speed tests together must run within 15 seconds.