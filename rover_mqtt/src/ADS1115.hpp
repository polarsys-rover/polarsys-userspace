#ifndef SRC_ADS1115_HPP_
#define SRC_ADS1115_HPP_

class ADS1115 {
public:
	virtual ~ADS1115() {};

	virtual bool init() = 0;
	virtual void fini() = 0;

	virtual bool initialized() const = 0;

	/* Start an ADC conversion. */
	virtual bool StartConversion() = 0;

	/* Get the battery voltage in millivolts.  */
	virtual bool WaitAndGetConversionResult(uint16_t *res) = 0;
};

#endif /* SRC_ADS1115_HPP_ */
