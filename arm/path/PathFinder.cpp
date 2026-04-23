#include "PathFinder.h"




PathFinder::PathFinder()
{



}
PathFinder::~PathFinder()
{
}

std::vector<std::vector<double>> PathFinder::findPath(std::vector<double> startPose, std::vector<double> koordinat, std::vector<double> lastJoint)
{
    // make a valid path with a start pos, end koordinate and a current joint configuration as paramters
    std::vector<std::vector<double>> path;
    path = resolutionpath(koordinat, startPose);
    path = executeblejoints(getPreferedInvseKinamtikSoltions(path), lastJoint);
    return path;
}


std::vector<std::vector<double>> PathFinder::resolutionpath(std::vector<double> koordinat, std::vector<double> startPose)
{
    //take an end point ad a start point and make at path between them with a x number of points as resultions
    // retuns a list of points to the end points
    std::vector<std::vector<double>> path;
    double dx = koordinat[0] - startPose[0];
    double dy = koordinat[1] - startPose[1];
    double dz = koordinat[2] - startPose[2];

    double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    int points = std::ceil(distance / 0.02);

    for (int i = 0; i <= points; i++)
    {
        double t = (double)i / points;
        double x = (startPose[0] + t * dx);
        double y = (startPose[1] + t * dy);
        double z = (startPose[2] + t * dz);
        std::vector<double> pointPose = { x, y, z, koordinat[3], koordinat[4], koordinat[5] };
        path.push_back(pointPose);
    }
    return path;

}

//std::vector<std::vector<double>> PathFinder::obstacalcheck(std::vector<std::vector<double>> path)
//{
   // std::vector<std::vector<double>> newPath;
    //for (int i = 0; i < obstiacals.size(); i++)
   // {
    //    newPath = obstiacals[i].objektAvoid(path, tcpRadius);
    //}
  //  return newPath;
//}

std::vector<std::vector<std::vector<double>>> PathFinder::getPreferedInvseKinamtikSoltions(std::vector<std::vector<double>> points)
{
    // take list point map and find all the inverse kinatik soltions to each point and if its empty it vil be push an emty list that indciade a singularty
    // if its not emty it uses solutuonSorter funktion to take sort all the soltuons that is valid for that given postions constrain and save the soltions in
    //  preferedJointsList
    std::vector<std::vector<std::vector<double>>>preferedJointsList;
    for (int i = 0; i < points.size(); i++)
    {
        std::vector <std::vector<double>> JointList = IK.getInvseKinamtiksList(points[i], false);
        if (JointList.empty())
        {
            std::cout << "emty" << std::endl;
            preferedJointsList.push_back({});

        }
        else
        {
            std::cout << "full" << std::endl;
            std::vector <std::vector<double>>sortetdList = solutuonSorter(JointList, "albowOp");
            preferedJointsList.push_back(sortetdList);

        }


    }
    return preferedJointsList;
}
std::vector<std::vector<double>> PathFinder::solutuonSorter(std::vector<std::vector<double>> JointList, std::string sol)
{
    // take a list of ik soltions a make a new list were alle the solutions that do meet the required postion 
    std::vector<std::vector<double>> sortetPointList;
    for (int i = 0; i < JointList.size(); i++)
    {
        if (sol == "albowOp")
        {
            if (JointList[i][1] > M_PI)
            {
                sortetPointList.push_back(JointList[i]);
            }
        }
    }
    return sortetPointList;

}
std::vector<std::vector<double>> PathFinder::executeblejoints(std::vector<std::vector<std::vector<double>>> aprovedJoints ,std::vector<double> lastJoint)
{
// This funkton takes a list of point were each poit have a list of kinamtik soltions were they are sortet so the corect solotions a in it
// after that it find the shortest way to the next point by looking at all the solotions at take the sum of them and the solotions with the smalist sum
// become the solotion and get saved in the list executeblejointslist
    std::vector<std::vector<double>> executeblejointslist;

    //the current joint is uses as the first refrence
    std::vector<double> lastSol = lastJoint;
    std::vector<std::vector<double>> normlist;

    for (int i = 0; i < aprovedJoints.size(); i++)
    {

        if (!aprovedJoints[i].empty())
        {
            std::vector<double> bestSol;
            double last = 10;

            for (int k = 0; k < aprovedJoints[i].size(); k++)
            {
                double sum = 0;
                for (int s = 0; s < 6; s++)
                {
                    double a = aprovedJoints[i][k][s];
                    double b = lastSol[s];
                    double diff = abs(a - b);
                    // norm diffe is the joint angle the other way around found by minus by pi
                    double aNorm = a - 2 * M_PI;
                    double diffnorm = abs(aNorm - b);

                    // if the the other way has a shorter rotaion it will be the new rotation
                    if (diff > diffnorm)
                    {
                        sum += diffnorm;
                        aprovedJoints[i][k][s] = aNorm;
                    }
                    else
                    {
                        sum += diff;
                    }
                }
                if (sum < last)
                {
                    last = sum;
                    bestSol = aprovedJoints[i][k];
                }
            }
            lastSol = bestSol;
            executeblejointslist.push_back(bestSol);
        }
    }
    return executeblejointslist;
}

