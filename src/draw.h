#ifndef DRAW_H
#define DRAW_H

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/math/distributions/binomial.hpp>

#include <htslib/sam.h>


namespace wallysworld
{

  

  inline int32_t
    pixelX(int32_t const width, int32_t const rgsz, int32_t const genomicX) {
    return (int32_t) (((double) genomicX / (double) rgsz) * width);
  }

  inline int32_t
    genomicX(int32_t const width, int32_t const rgsz, int32_t const pixelX) {
    return (int32_t) (((double) pixelX / (double) width) * rgsz);
  }

  inline uint32_t
  findTicks(double const pxoffset, double const textwidth) {
    std::vector<int32_t> bounds{5, 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    for(uint32_t i = 0; i < bounds.size(); ++i) {
      if (bounds[i] * pxoffset > textwidth * 1.5) {
	return bounds[i];
	break;
      }
    }
    return 1000000;
  }
    
  template<typename TConfig>
  inline void
  drawGenome(TConfig const& c, Region const& rg, cv::Mat& img, int32_t const track) {
    std::string text(boost::lexical_cast<std::string>(rg.end));
    int32_t n = text.length() - 3;
    while (n > 0) {
      text.insert(n, ",");
      n -= 3;
    }
    double font_scale = 0.4;
    double font_thickness = 1.5;
    int32_t baseline = 0;
    cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, font_scale, font_thickness, &baseline);

    // Find suitable tick size
    uint32_t modval = findTicks(c.pxoffset, textSize.width);
    cv::line(img, cv::Point(0, track * c.tlheight + c.tlheight), cv::Point(c.width, track * c.tlheight + c.tlheight), cv::Scalar(0, 0, 0), 1.8);
    double px = 0;
    for(int32_t i = rg.beg; i < rg.end; ++i) {
      if (i % modval == 0) {
	cv::line(img, cv::Point(px - c.pxoffset/2, track * c.tlheight), cv::Point(px - c.pxoffset/2, track * c.tlheight + c.tlheight), cv::Scalar(0, 0, 0), 1.8);
      }
      if (i % modval == 0) {
	// Font
	text = boost::lexical_cast<std::string>(i);
	n = text.length() - 3;
	while (n > 0) {
	  text.insert(n, ",");
	  n -= 3;
	}
	baseline = 0;
	cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, font_scale, font_thickness, &baseline);
	cv::putText(img, text, cv::Point(px - c.pxoffset/2 - textSize.width/2, (track - 1) * c.tlheight + textSize.height), cv::FONT_HERSHEY_DUPLEX, font_scale, cv::Scalar(0, 0, 0), font_thickness);
      }
      px += c.pxoffset;
    }
  }

  template<typename TConfig>
  inline void
  drawReference(TConfig const& c, Region const& rg, cv::Mat& img, std::string const& ref, int32_t const track) {
    if (c.pxoffset >= WALLY_PX) {
      double px = 0;
      double font_scale = 0.4;
      double font_thickness = 1.5;
      int32_t baseline = 0;
      for(int32_t i = 0; i < rg.size; ++i) {
	std::string text(1, ref[i]);
	cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, font_scale, font_thickness, &baseline);
	cv::putText(img, text, cv::Point(px + c.pxoffset/2 - textSize.width/2, track * c.tlheight + textSize.height + 1), cv::FONT_HERSHEY_DUPLEX, font_scale, cv::Scalar(0, 0, 0), font_thickness);
	px += c.pxoffset;
      }
    }
  }
  
  template<typename TConfig, typename TCoverage>
  inline void
  drawCoverage(TConfig const& c, Region const& rg, cv::Mat& img, TCoverage const& covA, TCoverage const& covC, TCoverage const& covG, TCoverage const& covT, std::vector<bool> const& snp, int32_t const track) {
    uint32_t maxObsCov = 0;
    for(uint32_t i = 0; i < covA.size(); ++i) {
      uint32_t cumsum = covA[i];
      cumsum += covC[i];
      cumsum += covG[i];
      cumsum += covT[i];
      if (cumsum > maxObsCov) maxObsCov = cumsum;
    }

    // Draw coverage histogram
    double px = 0;
    for(int32_t i = 0; i < rg.size; ++i) {
      uint32_t cumsum =	covA[i];
      cumsum += covC[i];
      cumsum += covG[i];
      cumsum += covT[i];
      double frac = (double) cumsum / (double) maxObsCov;
      int32_t ch = (int32_t) (frac * 2 * c.tlheight);
      if (c.pxoffset >= WALLY_PX) {
	if (!snp[i]) {
	  cv::Rect rect(px + 1, (track-1) * c.tlheight + 2 * c.tlheight - ch, c.pxoffset - 1, ch);
	  cv::rectangle(img, rect, cv::Scalar(200, 200, 200), -1);
	} else {
	  double frac = (double) covA[i] / (double) maxObsCov;
	  int32_t ch = (int32_t) (frac * 2 * c.tlheight);
	  int32_t cumCH = ch;
	  cv::Rect rectA(px + 1, (track-1) * c.tlheight + 2 * c.tlheight - cumCH, c.pxoffset - 1, ch);
	  cv::rectangle(img, rectA, WALLY_A, -1);
	  frac = (double) covC[i] / (double) maxObsCov;
	  ch = (int32_t) (frac * 2 * c.tlheight);
	  cumCH += ch;
	  cv::Rect rectC(px + 1, (track-1) * c.tlheight + 2 * c.tlheight - cumCH, c.pxoffset - 1, ch);
	  cv::rectangle(img, rectC, WALLY_C, -1);
	  frac = (double) covG[i] / (double) maxObsCov;
	  ch = (int32_t) (frac * 2 * c.tlheight);
	  cumCH += ch;
	  cv::Rect rectG(px + 1, (track-1) * c.tlheight + 2 * c.tlheight - cumCH, c.pxoffset - 1, ch);
	  cv::rectangle(img, rectG, WALLY_G, -1);
	  frac = (double) covT[i] / (double) maxObsCov;
	  ch = (int32_t) (frac * 2 * c.tlheight);
	  cumCH += ch;
	  cv::Rect rectT(px + 1, (track-1) * c.tlheight + 2 * c.tlheight - cumCH, c.pxoffset - 1, ch);
	  cv::rectangle(img, rectT, WALLY_T, -1);
	}
      } else {
	int32_t wi = (int32_t) (c.pxoffset) + 1;
	cv::Rect rect(px, (track-1) * c.tlheight + 2 * c.tlheight - ch, wi, ch);
	cv::rectangle(img, rect, cv::Scalar(200, 200, 200), -1);
      }
      px += c.pxoffset;
    }
  }

  
  template<typename TConfig>
  inline void
  drawRead(TConfig const& c, cv::Mat& img, int32_t const x, int32_t const y, int32_t const w, int32_t const h, bool const reverse, bool const tri) {
    cv::Rect rect(x, y, w, h);
    cv::rectangle(img, rect, cv::Scalar(200, 200, 200), -1);
    if (tri) {
      typedef std::vector<cv::Point> TPointVector;
      TPointVector pvec;
      if (reverse) {
	std::vector<cv::Point> pvec{cv::Point(x, y), cv::Point(x, y+h), cv::Point(x-c.pxoffset/3, y + h/2)};
	cv::polylines(img, pvec, true, cv::Scalar(200, 200, 200), 1);
	cv::fillPoly(img, pvec, cv::Scalar(200, 200, 200));
      } else {
	std::vector<cv::Point> pvec{cv::Point(x+w, y), cv::Point(x+w, y+h), cv::Point(x+w+c.pxoffset/3, y + h/2)};
	cv::polylines(img, pvec, true, cv::Scalar(200, 200, 200), 1);
	cv::fillPoly(img, pvec, cv::Scalar(200, 200, 200));
      }
    }
  }

  template<typename TConfig>
  inline void
  drawRead(TConfig const& c, Region const& rg, cv::Mat& img, int32_t const track, int32_t const gstart, int32_t const gend, bool const reverse, bool const tri) {
    int32_t px = pixelX(c.width, rg.size, gstart);
    int32_t pxend = pixelX(c.width, rg.size, gend);
    drawRead(c, img, px, track * c.tlheight, pxend - px, c.rdheight, reverse, tri);
  }

  template<typename TConfig>
  inline void
  drawNuc(TConfig const& c, cv::Mat& img, int32_t const x, int32_t const y, int32_t const w, int32_t const h, char const nuc) {
    // Background rectangle
    cv::Rect rect(x, y, w, h);

    // Font
    std::string text(1, nuc);
    double font_scale = 0.4;
    double font_thickness = 1.5;
    int32_t baseline = 0;
    cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, font_scale, font_thickness, &baseline);

    // Put nucleotide if there is space
    if (c.pxoffset >= WALLY_PX) {
      cv::rectangle(img, rect, cv::Scalar(200, 200, 200), -1);
      if ((nuc == 'a') or (nuc == 'A')) {
	cv::putText(img, text, cv::Point(x + w/2 - textSize.width/2, y+h/2+textSize.height/2), cv::FONT_HERSHEY_DUPLEX, font_scale, WALLY_A, font_thickness);
      }
      else if ((nuc == 'c') or (nuc == 'C')) {
      	cv::putText(img, text, cv::Point(x + w/2 - textSize.width/2, y+h/2+textSize.height/2), cv::FONT_HERSHEY_DUPLEX, font_scale, WALLY_C, font_thickness);
      }
      else if ((nuc == 'g') or (nuc == 'G')) {
	cv::putText(img, text, cv::Point(x + w/2 - textSize.width/2, y+h/2+textSize.height/2), cv::FONT_HERSHEY_DUPLEX, font_scale, WALLY_G, font_thickness);
      }
      else if ((nuc == 't') or (nuc == 'T')) {
	cv::putText(img, text, cv::Point(x + w/2 - textSize.width/2, y+h/2+textSize.height/2), cv::FONT_HERSHEY_DUPLEX, font_scale, WALLY_T, font_thickness);
      }
    } else { 
      if ((nuc == 'a') or (nuc == 'A')) {
	cv::rectangle(img, rect, WALLY_A, -1);
      }
      else if ((nuc == 'c') or (nuc == 'C')) {
	cv::rectangle(img, rect, WALLY_C, -1);
      }
      else if ((nuc == 'g') or (nuc == 'G')) {
	cv::rectangle(img, rect, WALLY_G, -1);
      }
      else if ((nuc == 't') or (nuc == 'T')) {
	cv::rectangle(img, rect, WALLY_T, -1);
      }
    }
  }
  
  template<typename TConfig>
  inline void
  drawNuc(TConfig const& c, Region const& rg, cv::Mat& img, int32_t const track, int32_t const gstart, int32_t const gend, char const nuc) {
    int32_t px = pixelX(c.width, rg.size, gstart);
    int32_t pxend = pixelX(c.width, rg.size, gend);
    drawNuc(c, img, px, track * c.tlheight, pxend - px, c.rdheight, nuc);
  }

  inline void
  drawDel(cv::Mat& img, int32_t const x, int32_t const y, int32_t const w, int32_t const h, int32_t const len) {
    std::string text = boost::lexical_cast<std::string>(len);
    double font_scale = 0.4;
    double font_thickness = 1.5;
    int32_t baseline = 0;
    cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_DUPLEX, font_scale, font_thickness, &baseline);
    cv::line(img, cv::Point(x, y+h/2), cv::Point(x+w, y+h/2), cv::Scalar(0, 0, 0), 1.8);
    double frac = (double) textSize.width / (double) w;
    // Put length if there is space
    if (frac < 0.5) {
      cv::Rect rect(x+w/2-textSize.width/2, y, textSize.width, h);
      cv::rectangle(img, rect, cv::Scalar(255, 255, 255), -1);
      cv::putText(img, text, cv::Point(x+w/2 - textSize.width/2, y + h/2 + textSize.height/2), cv::FONT_HERSHEY_SIMPLEX, font_scale, cv::Scalar(211, 0, 148), font_thickness);
    }
  }

  template<typename TConfig>
  inline void
    drawDel(TConfig const& c, Region const& rg, cv::Mat& img, int32_t const track, int32_t const gstart, int32_t const gend, int32_t const len) {
    int32_t px = pixelX(c.width, rg.size, gstart);
    int32_t pxend = pixelX(c.width, rg.size, gend);
    drawDel(img, px, track * c.tlheight, pxend - px, c.rdheight, len);
  }
  
}

#endif