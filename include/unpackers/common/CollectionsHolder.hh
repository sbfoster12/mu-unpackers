#ifndef COMMON_UNPACKING_COLLECTIONSHOLDER_HH
#define COMMON_UNPACKING_COLLECTIONSHOLDER_HH

//Standard
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

//ROOT
#include "TObject.h"

//Custom
#include <data_products/common/DataProduct.hh>

namespace unpackers {

    class CollectionsHolder : public TObject {
        
    public:
        
        //Constructor
        CollectionsHolder();

        //Destructor
        virtual ~CollectionsHolder();

        std::map<std::string,std::shared_ptr<dataProducts::DataProductPtrCollection>>
        GetCollections();

        // This template function returns a std::vector<T>
        // extracted from the collectionPtrs map. That map stores the vector of
        // base DataProduct pointers, so we have to individually cast 
        // each back to its original derived class. Here we return a vector
        // of the objects (no pointers)
        // The dynamic cast to T must be valid or else an empty vector is returned
        template <typename T>
        std::vector<T> GetCollection(std::string collectionName) {
            //Get the collection of base pointers
            auto colBasePtr = basePtrCols_[collectionName];
            //Create dervied object (not pointer) collection
            std::vector<T> derivedCol;
            //Loop over references to base pointers and cast to derived
            //Exit if casting fails
            //Return collection of derived objects (not pointers)
            //These derived objects are copies - is this okay?
            for (const auto& basePtr : *colBasePtr) {
                if (auto derivedPtr = dynamic_cast<T*>(basePtr.get())) {
                    derivedCol.push_back(*derivedPtr);
                } else {
                    std::cerr << "Collection could not be found with the provided template.\n"
                        << "Returning an empty vector\n";
                    return {};
                }
            }
            return derivedCol;
        }

        template <typename T, typename MembType>
        std::map<int,std::vector<T>> GetCollectionMap(std::string collectionName,MembType memb) {
            //Get the collection of base pointers
            auto colBasePtr = basePtrCols_[collectionName];
            //Create derived object (not pointer) collection
            std::map<int,std::vector<T>> derivedColMap;
            //Loop over references to base pointers and cast to derived
            //Exit if casting fails
            //Return collection of derived objects (not pointers)
            //These derived objects are copies - is this okay?
            for (const auto& basePtr : *colBasePtr) {
                if (auto derivedPtr = dynamic_cast<T*>(basePtr.get())) {
                    int index = derivedPtr->*memb;

                    if (derivedColMap.find(index) == derivedColMap.end()) {
                        // If it doesn't exist, create a new vector for this index
                        derivedColMap[index] = std::vector<T>();
                    }

                    derivedColMap[index].push_back(*derivedPtr);
                } else {
                    std::cerr << "Collection could not be found with the provided template.\n"
                        << "Returning an empty vector\n";
                    return {};
                }
            }
            return derivedColMap;
        }

        template <typename T, typename MembType>
        std::map<int,std::vector<std::shared_ptr<dataProducts::DataProduct>>> GetPtrCollectionMap(std::string collectionName,MembType memb) {
            //Get the collection of base pointers
            auto colBasePtr = basePtrCols_[collectionName];
            //Create a map of base pointers
            std::map<int,std::vector<std::shared_ptr<dataProducts::DataProduct>>> basePtrColMap;
            //Loop over references to base pointers and cast to derived to use MembType
            for (const auto& basePtr : *colBasePtr) {
                if (auto derivedPtr = dynamic_cast<T*>(basePtr.get())) {
                    int index = derivedPtr->*memb;

                    if (basePtrColMap.find(index) == basePtrColMap.end()) {
                        // If it doesn't exist, create a new vector for this index
                        basePtrColMap[index] = std::vector<std::shared_ptr<dataProducts::DataProduct>>();
                    }

                    basePtrColMap[index].push_back(basePtr);
                } else {
                    std::cerr << "Collection could not be found with the provided template.\n"
                        << "Returning an empty vector\n";
                    return {};
                }
            }
            return basePtrColMap;
        }

        template <typename T, typename MembType>
        std::vector<std::vector<T>> GetCollectionVector(std::string collectionName,MembType memb) {
            auto derivedColMap = GetCollectionMap<T>(collectionName,memb);
            std::vector<std::vector<T>> derivedColVector;
            for (const auto& pair : derivedColMap) {
                derivedColVector.push_back(pair.second);
            }
            return derivedColVector;
        }

        template <typename T, typename MembType>
        std::vector<std::vector<T>> GetPtrCollectionVector(std::string collectionName,MembType memb) {
            auto basePtrColMap = GetPtrCollectionMap<T>(collectionName,memb);
            std::vector<std::vector<std::shared_ptr<dataProducts::DataProduct>>> basePtrColVector;
            for (const auto& pair : basePtrColMap) {
                basePtrColVector.push_back(pair.second);
            }
            return basePtrColVector;
        }

        void ResetIterations() {
            currentIteration_ = -1;
            maxIterations_ = -1;
            iterationsMap_.clear();
        }

        template <typename T>
        void SetIterations(std::string collectionName) {

            //initialize internal state
            currentIteration_ = 0;
            maxIterations_ = -1;
            iterationsMap_.clear();

            // Get the collection of base pointers
            auto colBasePtr = basePtrCols_[collectionName];
            int iter = 0;
            auto iterMemb = T::iterMemb;
            for (const auto& basePtr : *colBasePtr) {
                if (auto derivedPtr = dynamic_cast<T*>(basePtr.get())) {
                    int index = derivedPtr->*iterMemb;
                    if (iterationsMap_.find(index) == iterationsMap_.end()) {
                        iterationsMap_[index] = iter;
                        iter++;
                    }
                    
                } else {
                    std::cerr << "Collection could not be found with the provided template.\n"
                        << "Returning an empty map\n";
                    return;
                }
            }
            maxIterations_ = iterationsMap_.size();
        }

        template <typename T>
        std::vector<T> GetNextCollection(std::string collectionName) {

            // First check if the current iteration is valid
            if (currentIteration_ >= maxIterations_ || currentIteration_ < 0) {
                return {};
            }

            //Create derived object (not pointer) collection
            std::vector<T> derivedCol;
            
            //Get the collection of base pointers
            auto basePtrCol = basePtrCols_[collectionName];

            // Loop over base pointers
            for (const auto& basePtr : *basePtrCol) {
                if (auto derivedPtr = dynamic_cast<T*>(basePtr.get())) {

                    // Check if the derived class has an iteration member
                    if (T::iterMemb) {
                        int index = derivedPtr->*(T::iterMemb);
                        int thisIteration = iterationsMap_[index];
                        
                        // if this is the correct index
                        if (thisIteration == currentIteration_) {
                            derivedCol.push_back(*derivedPtr);
                        }
                    } else {
                        // If no iteration member, just add the derived pointer (all data products are share across trigger events)
                        derivedCol.push_back(*derivedPtr);
                    }
                } else { // can't cast
                    std::cerr << "Collection could not be found with the provided template.\n"
                        << "Returning false\n";
                    return {};
                }
            }

            return derivedCol;
        }

        template <typename T>
        std::vector<std::shared_ptr<dataProducts::DataProduct>> GetNextPtrCollection(std::string collectionName) {

            // First check if the current iteration is valid
            if (currentIteration_ >= maxIterations_ || currentIteration_ < 0) {
                return {};
            }

            //Create base ptr collection for this iteration
            std::vector<std::shared_ptr<dataProducts::DataProduct>> thisBasePtrCol;

            //Get the collection of base pointers
            auto basePtrCol = basePtrCols_[collectionName];

            // Loop over base pointers
            for (const auto& basePtr : *basePtrCol) {
                if (auto derivedPtr = dynamic_cast<T*>(basePtr.get())) {

                    // Check if the derived class has an iteration member
                    if (T::iterMemb) {
                        int index = derivedPtr->*(T::iterMemb);
                        int thisIteration = iterationsMap_[index];
                        
                        // if this is the correct index
                        if (thisIteration == currentIteration_) {
                            thisBasePtrCol.push_back(basePtr);
                        }
                    } else {
                        // If no iteration member, just add the derived pointer (all data products are share across trigger events)
                        thisBasePtrCol.push_back(basePtr);
                    }
                } else { // can't cast
                    std::cerr << "Collection could not be found with the provided template.\n"
                        << "Returning false\n";
                    return {};
                }
            }

            return thisBasePtrCol;
        }

        void ClearCollections();

        void RegisterCollections(const std::map<std::string,std::shared_ptr<dataProducts::DataProductPtrCollection>>& basePtrCol);
        void RegisterCollection(const std::string label, const std::shared_ptr<dataProducts::DataProductPtrCollection> colPtr);

        template <typename T>
        std::unique_ptr<T>  MakeAndRegister() {
            std::unique_ptr<T> t = std::make_unique<T>();
            this->RegisterCollections(t->GetCollections());
            return t;
        }

        ClassDef(CollectionsHolder, 1);

    private:
        //class name
        const std::string className_;

    protected:

        //collections
        std::map<std::string,std::shared_ptr<dataProducts::DataProductPtrCollection>> basePtrCols_;

        // Internal state for unpacking "trigger" events
        int currentIteration_; //current iteration
        int maxIterations_; // Number of groups
        std::map<int,int> iterationsMap_; // Iteration map


    };
    // Define the unpacking status enum
    enum unpackingStatus { Success = 0, SuccessDone = 1, SuccessMore = 2, Failure = 3};
}

#endif // COMMON_UNPACKING_COLLECTIONSHOLDER_HH