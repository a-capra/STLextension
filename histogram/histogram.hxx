#pragma once

#include <map>
#include <vector>
#include <iomanip>
#include <cmath>

template <class T>
class histogram
{
public:
	histogram(int n, T min, T max): fNbins(n), fMin(min), fMax(max),
									fEntries(0),fMean(0.), fRMS(0.),
									fMaxContent(-1), fMostProbable(min), 
									fMedian(min),
									fUnderflow(0),fOverflow(0),
									fLast(false)
	{
		fStep = (fMax - fMin) / fNbins;
		T bin = fMin;
		while(bin < fMax)
		{
			fHisto[bin] = 0;
			bin += fStep;
		}
		std::cout << "histogram ctor with " << fNbins << " bins from " << fMin << " to " << fMax << std::endl;
	}

	void Fill(const std::vector<T>& v)
	{
		std::cout << "Filling histogram with " << v.size() << " entries" << std::endl;
		fLast = false;
		for(auto& x : v)
		{
			T bin = fMin;
			if (x < fMin)
			{
				++fUnderflow;
				continue;
			}
			else if (x > fMax)
			{
				++fOverflow;
				continue;
			}
			while(bin < fMax)
			{
				if (x >= bin && x < (bin + fStep))
				{
					++fHisto[bin];
					++fEntries;
					if (fHisto[bin] >= fMaxContent)
					{
						fMaxContent = fHisto[bin];
						fMostProbable = bin;
					}
					UpdateMean(bin);
					break;
				}
				bin += fStep;
			}
		}
		fLast = true;
		UpdateMean(fMin);
	}

	void Fill(const T& x)
	{
		fLast = false;
		if (x < fMin)
		{
			++fUnderflow;
			return;
		}
		else if (x > fMax)
		{
			++fOverflow;
			return;
		}
		T bin = fMin;
		while (bin < fMax)
		{
			if (x >= bin && x < (bin + fStep))
			{
				++fHisto[bin];
				++fEntries;
				if (fHisto[bin] >= fMaxContent)
				{
					fMaxContent = fHisto[bin];
					fMostProbable = bin;
				}
				UpdateMean(bin);
				return;
			}
			bin += fStep;
		}
	}

	void UpdateMean(T& x)
	{
		if (fLast)
		{
			fMean /= (double)fEntries;
			fRMS /= (double)fEntries;
			fRMS -= (fMean * fMean);
			fRMS = sqrt(fRMS);
			fLast = false;
			return;
		}
		fMean += x;
		fRMS += x * x;
	}

	void Median()
	{
		if (!fEntries)
		{
			std::cerr << "histogram::Stats Empty!" << std::endl;
		}
		double den = 0.;
		for (auto it = fHisto.begin(); it != fHisto.end(); ++it)
		{
			den += (double)it->second;
			if ((den / double(fEntries)) > 0.5) return;
			fMedian = it->first;
		}
	}

	void Stats()
	{
		Median();
		fLast = true;
		UpdateMean(fMin);
	}

	void Print(int norm=99) const
	{
		for(auto it = fHisto.begin(); it != fHisto.end(); ++it)
		{
			std::cout << std::setw(5) <<it->first << ": ";
			std::cout << std::string(int(it->second) * norm / fEntries, '*') << std::endl;
		}
	}

	void PrintStats() const
	{
		std::cout << "Most Probable Bin: " << GetMostProbable()
			<< "\tMean: " << GetMean()
			<< " RMS: " << GetRMS()
			<< "\t Median: " << GetMedian()
			<< "\t Entries: " << GetEntries() 
			<< " Underflow: "<< GetUnderflow()
			<< " Overflow: " << GetOverflow() << std::endl;
	}

	inline T GetMostProbable() const { return fMostProbable; }
	inline double GetMean() const { return fMean; }
	inline double GetRMS() const { return fRMS; }
	inline T GetMedian() const { return fMedian; }
	inline int GetEntries() const { return fEntries; }
	inline int GetUnderflow() const { return fUnderflow; }
	inline int GetOverflow() const { return fOverflow; }

private:
	std::map<T, int> fHisto;
	unsigned fNbins;
	T fMin;
	T fMax;
	T fStep;
	int fEntries;
	double fMean;
	double fRMS;
	T fMostProbable;
	int fMaxContent;
	T fMedian;
	int fUnderflow;
	int fOverflow;
	bool fLast;
};

