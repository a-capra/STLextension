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
									fMostProbable(min), fMedian(min),
									fUnderflow(0),fOverflow(0)
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
					break;
				}
				bin += fStep;
			}
		}
	}

	void Fill(const T& x)
	{
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
				return;
			}
			bin += fStep;
		}
	}

	void Stats()
	{
		if (!fEntries)
		{
			std::cerr << "histogram::Stats Empty!" << std::endl;
		}
		double den,max;
		den = max = 0.;
		for (auto it = fHisto.begin(); it != fHisto.end(); ++it)
		{
			if (it->second > max)
			{
				max = it->second;
				fMostProbable = it->first;
			}
			if( (den / double(fEntries)) <= 0.5) fMedian = it->first;
			den+=(double)it->second;
			fMean += it->first * it->second;
			fRMS += it->first * it->first * it->second;
		}
		fMean /= den;
		fRMS /= den;
		fRMS -= (fMean*fMean);
		fRMS = sqrt(fRMS);
	}

	void Print(int norm=99) const
	{
		for(auto it = fHisto.begin(); it != fHisto.end(); ++it)
		{
			std::cout << std::setw(5) <<it->first << ": ";
			std::cout << std::string(int(it->second) * norm / fEntries, '*') << std::endl;
		}
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
	T fMedian;
	int fUnderflow;
	int fOverflow;
};

